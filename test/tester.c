#include "emu.h"
#include "mem.h"
#include "cpu.h"
#include "flash.h"

#include <unistd.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>

void gui_do_stuff(void) {}
void gui_console_printf(const char *format, ...) {
#ifndef NDEBUG
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
#else
  (void)format;
#endif
}
void gui_emu_sleep(unsigned long us) { usleep(us); }
void throttle_timer_wait(void) {}

static const uint32_t stack = 0xD40000;
static const uint32_t retaddr = 0x3FFFFE;
static const uint8_t halt = 0166;

#define bitcast(dst, src, ...) ((union { src __src; dst __dst; }){ __VA_ARGS__ }.__dst)

static bool check(eZ80registers_t in, eZ80registers_t out, const char **reason) {
  (void)in;
  (void)out;
  if (!cpu.halted) {
    *reason = "Halt never reached";
    return false;
  }
  if (out.PC != retaddr + 1) {
    *reason = "Incorrect return address";
    return false;
  }
  if (mem_peek_long(stack - 3) != retaddr) {
    *reason = "Corrupted return address";
    return false;
  }
  if (out.SPL != in.SPL + 3) {
    *reason = "Corrupted stack pointer";
    return false;
  }
  *reason = "Check failed";
  return (CHECK);
}

int main(int argc, char **argv) {
  if (argc != 3) return 2;
  eZ80registers_t firstFailure;
  const char *firstReason;
  uint64_t failures = 0, iterations = 100000;
  emu_load(argv[1], NULL);
  flash.waitStates = 4;
  mem_poke_byte(retaddr, halt);
  int entry = atoi(argv[2]);
  mem_poke_byte(stack - 1, (uint8_t)(retaddr >> 16));
  mem_poke_byte(stack - 2, (uint8_t)(retaddr >>  8));
  mem_poke_byte(stack - 3, (uint8_t)(retaddr >>  0));
  for (uint64_t i = 0; i != iterations; i++) {
    cpu_flush(entry, 1);
    cpu.registers.SPL = stack - 3;
    cpu.halted = false;
    cpu.next = cpu.cycles + 10000;
    eZ80registers_t in = cpu.registers;
    cpu_execute();
    eZ80registers_t out = cpu.registers;
    const char *reason;
    if (!check(in, out, &reason) && failures++ == 0) {
      firstFailure = in;
      firstReason = reason;
    }
  }
  fprintf(stderr, " \33[%dm%" PRIu64 "/%" PRIu64
	  " failed\33[m in \33[33m%f cycles\33[m average\n",
	  failures ? 31 : 32, failures,
	  iterations, 1.0 / iterations * (cpu.cycles + cpu.cyclesOffset) - 4);
  if (failures)
    fprintf(stderr, "%s with input:\n"
	    "AF %04X     %04X AF'\n"
	    "BC %06X %06X BC'\n"
	    "DE %06X %06X DE'\n"
	    "HL %06X %06X HL'\n"
	    "IX %06X %06X SP\n"
	    "IY %06X %06X PC\n",
	    firstReason,
	    firstFailure.AF, firstFailure._AF,
	    firstFailure.BC, firstFailure._BC,
	    firstFailure.DE, firstFailure._DE,
	    firstFailure.HL, firstFailure._HL,
	    firstFailure.IX, firstFailure.SPL,
	    firstFailure.IY, firstFailure.PC);
  emu_cleanup();
  return failures != 0;
}
