#include "asic.h"
#include "cpu.h"
#include "emu.h"
#include "flash.h"
#include "mem.h"
#include "sched.h"

#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STACK_FLOAT 1

void gui_throttle(void) {}
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

static const uint32_t stack = 0xD40000;
static const uint32_t retaddr = 0x3FFFFE;
static const uint8_t halt = 0166;

typedef struct pair8_24 { uint32_t x : 24, y : 8; } pair8_24_t;
typedef struct tuple16_24_24 { uint64_t x : 24, y : 24, z : 16; } tuple16_24_24_t;
#define bitcast(dst, src, ...) ((union { src __src; dst __dst; }){ __VA_ARGS__ }.__dst)

bool same(float x, float y) {
  return (x == y && !signbit(x) == !signbit(y)) || (isnan(x) && isnan(y));
}

bool sameignzerosign(float x, float y) {
    return x == y || (isnan(x) && isnan(y));
}

float quiet(float x) {
    return isnan(x) ? NAN : x;
}

static bool prereq(eZ80registers_t in, uint8_t inStack[10][3]) {
  (void)in;
  (void)inStack;
  return (PREREQ);
}

static bool check(eZ80registers_t in, uint8_t inStack[10][3], eZ80registers_t out, uint8_t outStack[10][3], const char **reason) {
  (void)in, (void)inStack, (void)out, (void)outStack;
  if (!cpu.halted) {
    *reason = "Halt never reached";
    return false;
  }
  if (!cpu.ADL) {
    *reason = "In Z80 mode";
    return false;
  }
  if (cpu.MADL) {
    *reason = "MADL set";
    return false;
  }
  if (out.MBASE != 0xD0) {
    *reason = "MBASE changed";
    return false;
  }
  if (out.PC != retaddr + 1) {
    *reason = "Incorrect return address";
    return false;
  }
  if (out.SPL != in.SPL + 3) {
    *reason = "Corrupted stack pointer";
    return false;
  }
  *reason = "Check failed";
  return (CHECK);
}

static uint64_t rotl64(uint64_t x, int k) {
  return k ? (x << k) | (x >> (64 - k)) : x;
}

uint64_t random64(void) {
  static uint64_t state[2] = { UINT64_C(0xe7b8ff027f9caf84), UINT64_C(0x4f0dc5c99524be96) };
  const uint64_t state0 = state[0];
  uint64_t state1 = state[1];
  const uint64_t result = state0 + state1;

  state1 ^= state0;
  state[0] = rotl64(state0, 55) ^ state1 ^ (state1 << 14); // a, b
  state[1] = rotl64(state1, 36); // c

  return result;
}

static uint8_t random_edge(uint8_t type, uint8_t value) {
  static const uint8_t edges[] = { 0x00, 0x00, 0x01, 0x3F, 0x40, 0x7F, 0x7F, 0x80, 0x80, 0xBF, 0xC0, 0xFE, 0xFF, 0xFF };
  return type < sizeof(edges) ? edges[type] : value;
}
static uint32_t random_reg(uint8_t type) {
  uint64_t value = random64();
  if (type & 1) return value >> 40;
  return random_edge(value >> 60 & 15, value >> 52) << 16 |
         random_edge(value >> 48 & 15, value >> 40) << 8 |
         random_edge(value >> 36 & 15, value >> 28);
}
static void print_regs(eZ80registers_t *regs, uint8_t stack[10][3]) {
  fprintf(stderr,
          "\tAF %04X     %04X AF' (SP+00) %06X\n"
          "\tBC %06X %06X BC' (SP+03) %06X\n"
          "\tDE %06X %06X DE' (SP+06) %06X\n"
          "\tHL %06X %06X HL' (SP+09) %06X\n"
          "\tIX %06X %06X SP  (SP+12) %06X\n"
          "\tIY %06X %06X PC  (SP+15) %06X\n"
#if STACK_FLOAT
          "\t(3) %-+15.8e  (SP+18) %06X\n"
#else
          "\tABC %-+15.8e  (SP+18) %06X\n"
#endif
          "\tEHL %-+15.8e  (SP+21) %06X\n"
          "\tABC %-+16.6a (SP+24) %06X\n"
          "\tEHL %-+16.6a (SP+27) %06X\n",
          regs->AF, regs->_AF,
          stack[0][2] << 16 | stack[0][1] << 8 | stack[0][0],
          regs->BC, regs->_BC,
          stack[1][2] << 16 | stack[1][1] << 8 | stack[1][0],
          regs->DE, regs->_DE,
          stack[2][2] << 16 | stack[2][1] << 8 | stack[2][0],
          regs->HL, regs->_HL,
          stack[3][2] << 16 | stack[3][1] << 8 | stack[3][0],
          regs->IX, regs->SPL,
          stack[4][2] << 16 | stack[4][1] << 8 | stack[4][0],
          regs->IY, regs->PC,
          stack[5][2] << 16 | stack[5][1] << 8 | stack[5][0],
#if STACK_FLOAT
          *(float*)stack[1],
#else
          bitcast(float, pair8_24_t, { regs->BC, regs->A }),
#endif
          stack[6][2] << 16 | stack[6][1] << 8 | stack[6][0],
          bitcast(float, pair8_24_t, { regs->HL, regs->E }),
          stack[7][2] << 16 | stack[7][1] << 8 | stack[7][0],
          bitcast(float, pair8_24_t, { regs->BC, regs->A }),
          stack[8][2] << 16 | stack[8][1] << 8 | stack[8][0],
          bitcast(float, pair8_24_t, { regs->HL, regs->E }),
          stack[9][2] << 16 | stack[9][1] << 8 | stack[9][0]);
}

static void hint(eZ80registers_t in, uint8_t inStack[10][3], eZ80registers_t out, uint8_t outStack[10][3]) {
  (void)in, (void)inStack, (void)out, (void)outStack;
  //fprintf(stderr, "%016lX\n", bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }));
  //fprintf(stderr, "%08X %-+15.8e %-+16.6a\n", bitcast(uint32_t, float, x), x, x);
  //fprintf(stderr, "%g / %g -> %g != %g;\n", in.HL * 0x1p-12, in.DE * 0x1p-12, out.HL * 0x1p-12, ((((uint64_t)in.HL << 13) / in.DE + 0) >> 1) * 0x1p-12);
  //fprintf(stderr, "%.8g / %.8g -> %.8g != %.8g;\n", ((int32_t)((uint32_t)in.HL << 8) >> 8) * 0x1p-12, ((int32_t)((uint32_t)in.DE << 8) >> 8) * 0x1p-12, ((int32_t)((uint32_t)out.HL << 8) >> 8) * 0x1p-12, (((int32_t)((uint32_t)in.HL << 8) >> 8) * 0x1p-12) / (((int32_t)((uint32_t)in.DE << 8) >> 8) * 0x1p-12));
  //fprintf(stderr, "%016lX -> %016lX\n", bitcast(uint64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }), bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }));
  //fprintf(stderr, "%016lX -> %016lX != %016lX\n", *(int64_t*)inStack[1], bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }), labs(*(int64_t*)inStack[1]));
  //fprintf(stderr, "%016lX %% %016lX -> %016lX != %016lX\n", bitcast(int64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }), *(int64_t*)inStack[1], bitcast(uint64_t, tuple16_24_24_t, { out.HL, out.DE, out.BCS }), bitcast(int64_t, tuple16_24_24_t, { in.HL, in.DE, in.BCS }) % *(int64_t*)inStack[1]);
  //fprintf(stderr, "roundf(%-+15.8e) = %-+15.8e != %-+15.8e\n", *(float*)inStack[1], roundf(*(float*)inStack[1]), bitcast(float, pair8_24_t, { out.HL, out.E }));
  //fprintf(stderr, "%08X\n", bitcast(uint32_t, float, roundf(*(float*)inStack[1])));
}

#define UINT64_C_(x) UINT64_C(x)

int main(int argc, char **argv) {
  if (argc != 3) return 2;
  uint64_t valid = 0, failures = 0, firstFailure, minCycles = ~UINT64_C(0), maxCycles = UINT64_C(0);
  eZ80registers_t firstIn, firstOut;
  const char *firstReason;
  emu_load(false, argv[1]);
  cpu.cycles = 0;
  flash.waitStates = 4;
  mem_poke_byte(retaddr, halt);
  int entry = atoi(argv[2]);
  uint8_t inStack[10][3], firstInStack[10][3], firstOutStack[10][3];
  inStack[0][0] = (uint8_t)(retaddr >>  0);
  inStack[0][1] = (uint8_t)(retaddr >>  8);
  inStack[0][2] = (uint8_t)(retaddr >> 16);
  cpu.registers.MBASE = 0xD0;
  for (uint64_t i = 0; i != UINT64_C_(OFFSET) * 13; i++)
    random64();
  for (uint64_t i = UINT64_C_(OFFSET); i != UINT64_C_(ITERATIONS); i++) {
    uint64_t lastCycles = sched_total_cycles() - cpu.haltCycles;
    cpu.registers.AF = random_reg(i);
    cpu.registers._AF = random_reg(i);
    cpu.registers.BC = random_reg(i);
    cpu.registers._BC = random_reg(i);
    cpu.registers.DE = random_reg(i);
    cpu.registers._DE = random_reg(i);
    cpu.registers.HL = random_reg(i);
    cpu.registers._HL = random_reg(i);
    cpu.registers.IX = random_reg(i);
    cpu.registers.IY = random_reg(i);
    for (int index = 1; index <= 3; ++index) {
        uint32_t value = random_reg(i);
        inStack[index][0] = (uint8_t)(value >>  0);
        inStack[index][1] = (uint8_t)(value >>  8);
        inStack[index][2] = (uint8_t)(value >> 16);
    }
    cpu.registers.SPL = stack - 3;
    memcpy(phys_mem_ptr(cpu.registers.SPL, sizeof inStack), inStack, sizeof inStack);
    if (!prereq(cpu.registers, inStack))
        continue;
    ++valid;
    cpu.inBlock = cpu.halted = false;
    cpu.baseCycles += cpu.cycles;
    cpu.cycles = 0;
    sched.event.cycle = MAX_CYCLES;
    cpu_restore_next();
    eZ80registers_t in = cpu.registers;
    cpu_flush(entry, 1);
    cpu_execute();
    eZ80registers_t out = cpu.registers;
    uint8_t outStack[10][3];
    virt_mem_cpy(&outStack, out.SPL, sizeof outStack);
    const char *reason;
    if (!check(in, inStack, out, outStack, &reason) && failures++ == 0) {
      firstFailure = i;
      firstIn = in;
      firstOut = out;
      memcpy(firstInStack, inStack, sizeof inStack);
      memcpy(firstOutStack, outStack, sizeof outStack);
      firstReason = reason;
    }
    uint64_t curCycles = sched_total_cycles() - cpu.haltCycles - lastCycles;
    if (minCycles > curCycles) minCycles = curCycles;
    if (maxCycles < curCycles) maxCycles = curCycles;
  }
  fprintf(stderr, " \33[%dm%.6f%% failed\33[m in \33[33m%" PRIu64 "/%f/%" PRIu64 " cycles\33[m minimum/average/maximum\n",
          failures ? 31 : 32, 100.0 * failures / valid,
          minCycles, 1.0 / valid * (sched_total_cycles() - cpu.haltCycles), maxCycles);
  if (failures) {
    fprintf(stderr, "%s for test #%" PRIu64 " with input:\n", firstReason, firstFailure);
    print_regs(&firstIn, firstInStack);
    fprintf(stderr, "and output:\n");
    print_regs(&firstOut, firstOutStack);
    //hint(bitcast(float, pair8_24_t, { firstIn.BC, firstIn.A }) * bitcast(float, pair8_24_t, { firstIn.HL, firstIn.E }));
    //hint(bitcast(float, pair8_24_t, { firstIn.BC, firstIn.A }) / bitcast(float, pair8_24_t, { firstIn.HL, firstIn.E }));
    hint(firstIn, firstInStack, firstOut, firstOutStack);
  }
  asic_free();
  return failures != 0;
}
