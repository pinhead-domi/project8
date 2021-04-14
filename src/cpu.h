//
// Created by Dominik Haring on 08.04.2021.
//

#ifndef PROJECT8_CPU_H
#define PROJECT8_CPU_H

#include <inttypes.h>

typedef struct cpu_t{
    uint16_t pc;
    uint8_t sp;
    uint8_t v [16];
    uint16_t i;
    uint8_t dt, st;
    uint16_t stack [16];
    uint8_t memory [4096];
    uint8_t vram [64*32];
}cpu;

cpu Cpu_new();
void Cpu_cycle(cpu* c);

/*
void Cpu_SYS(cpu* c, uint16_t nnn);
void Cpu_CLS(cpu* c);
void Cpu_RET(cpu* c);
void Cpu_JUMP(cpu* c, uint16_t nnn);
void Cpu_CALL(cpu* c, uint16_t nnn);
void Cpu_SE(cpu* c, uint8_t x, uint8_t kk);
void Cpu_SNE(cpu* c, uint8_t x, uint8_t kk);
void Cpu_SEV(cpu* c, uint8_t x, uint8_t y);
void Cpu_LD(cpu* c, uint8_t y, uint8_t kk);
void Cpu_ADD(cpu* c, uint8_t x, uint8_t kk);
void Cpu_LDV(cpu* c, uint8_t x, uint8_t y);
void Cpu_OR(cpu* c, uint8_t x, uint8_t y);
void Cpu_AND(cpu* c, uint8_t x, uint8_t y);
void Cpu_XOR(cpu* c, uint8_t x, uint8_t y);
void Cpu_ADDC(cpu* c, uint8_t x, uint8_t y);
void Cpu_SUB(cpu* c, uint8_t x, uint8_t y);
void Cpu_SHR(cpu* c, uint8_t x);
void Cpu_SUBN(cpu* c, uint8_t x, uint8_t y);
void Cpu_SHL(cpu* c, uint8_t x);
void Cpu_SNEV(cpu* c, uint8_t x, uint8_t y);
void Cpu_LDI(cpu* c, uint16_t nnn);
void Cpu_JUMPV(cpu* c, uint16_t nnn);
void Cpu_RND(cpu* c, uint8_t x, uint8_t kk);
 */

#endif //PROJECT8_CPU_H
