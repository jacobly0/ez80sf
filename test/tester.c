#include "emu.h"
#include "mem.h"
#include "cpu.h"
#include "flash.h"

#include <inttypes.h>
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

#define bitcast(dst, src, ...) ((union { src __src; dst __dst; }){ __VA_ARGS__ }.__dst)

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

static uint8_t random_edge(uint8_t type, uint8_t value) {
  static const uint8_t edges[] = { 0x00, 0x01, 0x7F, 0x80, 0xFE, 0xFF };
  return type < sizeof(edges) ? edges[type] : value;
}
static uint32_t random_reg(struct random_data *random_data) {
  int32_t value;
  if (random_r(random_data, &value)) {
    fprintf(stderr, "random_r failed?!");
    exit(1);
  }
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

  struct random_data random_data;
  random_data.state = NULL;
  char random_state[] = { // chosen by fair dice roll.
    0xc4, 0x08, 0x21, 0x70, 0xee, 0x27, 0x67, 0x2d, 0x0b, 0x25, 0xdc, 0x0c, 0x2e, 0x3d, 0x9f, 0x5a,
    0x73, 0x65, 0xa3, 0xd3, 0xe1, 0x0f, 0x8c, 0xed, 0x9a, 0xed, 0x57, 0x3f, 0xd0, 0xa8, 0x23, 0x9a,
    0x00, 0xaa, 0x1a, 0x8d, 0xe3, 0x92, 0xcc, 0xc2, 0x05, 0x7e, 0x29, 0x14, 0xbf, 0x3b, 0xa3, 0x73,
    0xd4, 0x79, 0x4f, 0x07, 0x0c, 0x17, 0x8c, 0xd7, 0xa7, 0x63, 0x3f, 0x16, 0xd7, 0x54, 0xc7, 0x20,
    0x1d, 0x7c, 0x86, 0xd4, 0x01, 0xdb, 0x33, 0x5d, 0xe3, 0x9b, 0xbf, 0x54, 0x97, 0x7d, 0x48, 0xd5,
    0x39, 0xc5, 0x9f, 0x06, 0xc1, 0x02, 0x0a, 0x1b, 0xcc, 0xa9, 0xc7, 0x71, 0x62, 0x3e, 0xe2, 0x22,
    0xf0, 0xc2, 0x41, 0xf6, 0x8c, 0x51, 0x04, 0x20, 0x7e, 0xfa, 0xbb, 0xde, 0xc9, 0x45, 0x34, 0xdf,
    0x03, 0xb0, 0x64, 0x30, 0x1a, 0xf1, 0x2c, 0x49, 0x00, 0x7c, 0xd0, 0xe9, 0x4c, 0x27, 0xed, 0xdb,
    0x95, 0x93, 0x0d, 0x04, 0xa5, 0xe2, 0xc4, 0x5a, 0xfe, 0x98, 0xc4, 0x52, 0x6f, 0xd2, 0xda, 0xca,
    0xf7, 0xbb, 0x20, 0xd6, 0xee, 0x03, 0xb7, 0xda, 0xe5, 0xbe, 0x67, 0xf1, 0x92, 0x44, 0xea, 0xbd,
    0xa3, 0xcd, 0x50, 0x0a, 0x2b, 0x44, 0x0c, 0xe4, 0x00, 0x37, 0x35, 0x57, 0xc7, 0xc9, 0x33, 0x83,
    0xb8, 0xc7, 0x26, 0x90, 0x95, 0xfe, 0x97, 0xbd, 0xaa, 0xcc, 0xcb, 0x56, 0xf4, 0x8a, 0x3d, 0x7d,
    0x49, 0xc4, 0xdc, 0x1c, 0x77, 0x12, 0x41, 0xff, 0xdf, 0x99, 0x40, 0xe0, 0x90, 0xec, 0xb0, 0x3d,
    0x4c, 0xf5, 0x71, 0x93, 0x03, 0x7b, 0x53, 0xde, 0x76, 0xf6, 0x42, 0x60, 0x58, 0x31, 0xb1, 0x56,
    0x7e, 0x93, 0xb4, 0x6a, 0xe6, 0x80, 0xf6, 0x66, 0x09, 0x05, 0x78, 0x67, 0x33, 0x55, 0x87, 0x52,
    0x86, 0x02, 0xab, 0xd0, 0xe4, 0xb6, 0x88, 0x1a, 0xa2, 0x7a, 0x6f, 0x34, 0xcf, 0x8b, 0x09, 0x68
  }; // guaranteed to be random
  if (initstate_r(0x6b38295b, random_state, sizeof(random_state), &random_data)) {
    fprintf(stderr, "initstate_r failed?!");
    exit(1);
  }

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
    cpu.registers.AF = random_reg(&random_data);
    cpu.registers._AF = random_reg(&random_data);
    cpu.registers.BC = random_reg(&random_data);
    cpu.registers._BC = random_reg(&random_data);
    cpu.registers.DE = random_reg(&random_data);
    cpu.registers._DE = random_reg(&random_data);
    cpu.registers.HL = random_reg(&random_data);
    cpu.registers._HL = random_reg(&random_data);
    cpu.registers.IX = random_reg(&random_data);
    cpu.registers.IY = random_reg(&random_data);
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
