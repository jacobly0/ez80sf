#include "emu.h"
#include "mem.h"
#include "cpu.h"
#include "flash.h"

#include <unistd.h>
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

static bool check(eZ80registers_t in, eZ80registers_t out) {
  (void)in;
  (void)out;
  return (CHECK);
}

int main(int argc, char **argv) {
  if (argc != 3) return 2;
  uint64_t failures = 0;
  emu_load(argv[1], NULL);
  flash.waitStates = 4;
  mem_poke_byte(retaddr, halt);
  mem_poke_byte(stack - 1, (uint8_t)(retaddr >> 16));
  mem_poke_byte(stack - 2, (uint8_t)(retaddr >>  8));
  mem_poke_byte(stack - 3, (uint8_t)(retaddr >>  0));
  cpu_flush(atoi(argv[2]), 1);;
  cpu.registers.SPL = stack - 3;
  cpu.cycles = 0;
  cpu.next = 10000;
  eZ80registers_t in = cpu.registers;
  cpu_execute();
  eZ80registers_t out = cpu.registers;
  printf(" %s %ld cycles\n", cpu.registers.PC == retaddr + 1 && cpu.registers.SPL == stack ? check(in, out) ? "passed in" : "failed in" : "timed out after", cpu.cycles + cpu.cyclesOffset);
  emu_cleanup();
  return failures != 0;
}
