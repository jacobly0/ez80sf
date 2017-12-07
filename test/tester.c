#include "emu.h"
#include "mem.h"
#include "cpu.h"
#include "flash.h"

#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

typedef struct pair8_24 { uint32_t x : 24; uint8_t y : 8; } pair8_24_t;
#define bitcast(dst, src, ...) ((union { src __src; dst __dst; }){ __VA_ARGS__ }.__dst)
bool same(float x, float y) {
  return !((bitcast(uint32_t, float, x) ^ bitcast(uint32_t, float, y)) &
	   (isunordered(x, y) ? ~(1u << 22) : ~0u));
}

static bool check(eZ80registers_t in, eZ80registers_t out, const char **reason) {
  (void)in;
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

static uint32_t random32(void) {
  static uint64_t state[2] = { UINT64_C(0xe7b8ff027f9caf84), UINT64_C(0x4f0dc5c99524be96) };
  uint64_t x = state[0], y = state[1];
  state[0] = y;
  x ^= x << 23;
  return ((state[1] = x ^ y ^ (x >> 17) ^ (y >> 26)) + y) >> 32;
}
static uint8_t random_edge(uint8_t type, uint8_t value) {
  static const uint8_t edges[] = { 0x00, 0x01, 0x7F, 0x80, 0xFE, 0xFF };
  return type < sizeof(edges) ? edges[type] : value;
}
static uint32_t random_reg(void) {
  uint32_t value = random32();
  if ((value & 0xEE000000) == 0xEE000000)
    return value & 0x800000;
  return (value & 0xFF0000) |
    random_edge(value >> 28 & 0xF, value >> 8 & 0xFF) << 8 |
    random_edge(value >> 24 & 0xF, value >> 0 & 0xFF);
}
static void print_regs(eZ80registers_t *regs) {
  fprintf(stderr,
	  "\tAF %04X     %04X AF'\n"
	  "\tBC %06X %06X BC'\n"
	  "\tDE %06X %06X DE'\n"
	  "\tHL %06X %06X HL'\n"
	  "\tIX %06X %06X SP\n"
	  "\tIY %06X %06X PC\n"
	  "\tABC %e\n"
	  "\tEHL %e\n",
	  regs->AF, regs->_AF,
	  regs->BC, regs->_BC,
	  regs->DE, regs->_DE,
	  regs->HL, regs->_HL,
	  regs->IX, regs->SPL,
	  regs->IY, regs->PC,
	  bitcast(float, pair8_24_t, { regs->BC, regs->A }),
	  bitcast(float, pair8_24_t, { regs->HL, regs->E }));
}

int main(int argc, char **argv) {
  if (argc != 3) return 2;
  uint64_t failures = 0, iterations = 10, firstFailure;
  eZ80registers_t firstIn, firstOut;
  const char *firstReason;
  emu_load(argv[1], NULL);
  flash.waitStates = 4;
  mem_poke_byte(retaddr, halt);
  int entry = atoi(argv[2]);
  mem_poke_byte(stack - 1, (uint8_t)(retaddr >> 16));
  mem_poke_byte(stack - 2, (uint8_t)(retaddr >>  8));
  mem_poke_byte(stack - 3, (uint8_t)(retaddr >>  0));
  for (uint64_t i = 0; i != iterations; i++) {
    cpu.registers.AF = random_reg();
    cpu.registers._AF = random_reg();
    cpu.registers.BC = random_reg();
    cpu.registers._BC = random_reg();
    cpu.registers.DE = random_reg();
    cpu.registers._DE = random_reg();
    cpu.registers.HL = random_reg();
    cpu.registers._HL = random_reg();
    cpu.registers.IX = random_reg();
    cpu.registers.IY = random_reg();
    cpu.registers.SPL = stack - 3;
    cpu_flush(entry, 1);
    cpu.inBlock = cpu.halted = false;
    cpu.cyclesOffset += cpu.cycles;
    cpu.cycles = 0;
    cpu.next = 10000;
    eZ80registers_t in = cpu.registers;
    cpu_execute();
    eZ80registers_t out = cpu.registers;
    const char *reason;
    if (!check(in, out, &reason) && failures++ == 0) {
      firstFailure = i;
      firstIn = in;
      firstOut = out;
      firstReason = reason;
    }
  }
  fprintf(stderr, " \33[%dm%" PRIu64 "/%" PRIu64
	  " failed\33[m in \33[33m%f cycles\33[m average\n",
	  failures ? 31 : 32, failures,
	  iterations, 1.0 / iterations * (cpu.cycles + cpu.cyclesOffset) - 4);
  if (failures) {
    fprintf(stderr, "%s for test #%" PRIu64 " with input:\n", firstReason, firstFailure);
    print_regs(&firstIn);
    fprintf(stderr, "and output:\n");
    print_regs(&firstOut);
    fprintf(stderr, "%08" PRIX32 "\n", bitcast(uint32_t, float, bitcast(float, pair8_24_t, { firstIn.BC, firstIn.A }) + bitcast(float, pair8_24_t, { firstIn.HL, firstIn.E })));
  }
  emu_cleanup();
  return failures != 0;
}
