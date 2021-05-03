//
// Created by Dominik Haring on 08.04.2021.
//

#include <inttypes.h>

#include "cpu.h"
#include <time.h>
#include <stdlib.h>

cpu Cpu_new(){
    cpu c;
    c.pc=0;
    c.sp=0;
    c.i=0;

    c.dt=0;
    c.st=0;

    for (int i = 0; i < sizeof c.v; i++) {
        c.v[i]=0;
    }
    for (int i = 0; i < sizeof c.memory; i++) {
        c.memory[i]=0;
    }
    for (int i = 0; i < sizeof c.stack; i++) {
        c.stack[i]=0;
    }

    srand(time(NULL));

    return c;
}

void Cpu_SYS(cpu* c, uint16_t nnn){
    c->pc = nnn;
}

/*
 * 0x00E0 - CLS
 */
void Cpu_CLS(cpu* c){
    //ToDo: Clear display
}

/*
 * 0x00EE - RET
 */
void Cpu_RET(cpu* c){
    //c->sp -= 1;
    c->pc = c->stack[--c->sp];
}

/*
 * 0x1nnn - JUMP
 */
void Cpu_JUMP(cpu* c, uint16_t nnn){
    c->pc = nnn;
}

/*
 * 0x2nnn - CALL
 */
void Cpu_CALL(cpu* c, uint16_t nnn){
    c->stack[c->sp++] = c->pc;
    c->pc = nnn;
}

/*
 * 0x3xkk
 */
void Cpu_SE(cpu* c, uint8_t x, uint8_t kk) {
    if (c->v[x] == kk)
        c->pc += 2;
}

/*
 * 0x4xkk
 */
void Cpu_SNE(cpu* c, uint8_t x, uint8_t kk) {
    if (c->v[x] != kk)
        c->pc += 2;
}

/*
 * 0x5xy0
 */
void Cpu_SEV(cpu* c, uint8_t x, uint8_t y){
    if(c->v[x] == c->v[y])
        c->pc += 2;
}

/*
 * 0x6xkk
 */
void Cpu_LD(cpu* c, uint8_t x, uint8_t kk){
    c->v[x]=kk;
}

/*
 * 0x7xkk
 */
void Cpu_ADD(cpu* c, uint8_t x, uint8_t kk){
    c->v[x] += kk;
}

/*
 * 0x8xy0
 */
void Cpu_LDV(cpu* c, uint8_t x, uint8_t y){
    c->v[x]=c->v[y];
}

/*
 * 0x8xy1
 */
void Cpu_OR(cpu* c, uint8_t x, uint8_t y){
    c->v[x] = c->v[x] | c->v[y];
}

/*
 * 0x8xy2
 */
void Cpu_AND(cpu* c, uint8_t x, uint8_t y){
    c->v[x] = c->v[x] & c->v[y];
}

/*
 * 0x8xy3
 */
void Cpu_XOR(cpu* c, uint8_t x, uint8_t y){
    c->v[x] = c->v[x] ^ c->v[y];
}

/*
 * 0x8xy4
 */
void Cpu_ADDC(cpu* c, uint8_t x, uint8_t y){
    uint16_t sum = c->v[x] + c->v[y];
    if(sum > 255)
        c->v[0xF]=1;
    c->v[x] = (uint8_t) sum;
}

/*
 * 0x8xy5
 */
void Cpu_SUB(cpu* c, uint8_t x, uint8_t y){
    if(c->v[x] > c->v[y])
        c->v[0xF]=1;
    c->v[x] -= c->v[y];
}

/*
 * 0x8xy6
 */
void Cpu_SHR(cpu* c, uint8_t x){
    c->v[0xF] = c->v[x] & 1;
    c->v[x] = c->v[x]>>1;
}

/*
 * 0x8xy7
 */
void Cpu_SUBN(cpu* c, uint8_t x, uint8_t y){
    if(c->v[y] > c->v[x])
        c->v[0xF]=1;
    c->v[x] = c->v[y] - c->v[x];
}

/*
 * 0x8xyE
 */
void Cpu_SHL(cpu* c, uint8_t x){
    c->v[0xF]=(c->v[x] & 0b10000000) >> 7;
    c->v[x] = c->v[x]<<1;
}

/*
 * 0x9xy0
 */
void Cpu_SNEV(cpu* c, uint8_t x, uint8_t y){
    if(c->v[x] != c->v[y])
        c->pc += 2;
}

/*
 * 0xAnnn
 */
void Cpu_LDI(cpu* c, uint16_t nnn){
    c->i = nnn;
}

/*
 * 0xBnnn
 */
void Cpu_JUMPV(cpu* c, uint16_t nnn) {
    c->pc = nnn + c->v[0];
}

/*
 * 0xCxkk
 */
void Cpu_RND(cpu* c, uint8_t x, uint8_t kk){
    uint8_t r = rand();
    c->v[x] = r & kk;
}

void Cpu_DRW(cpu* c, uint8_t x, uint8_t y, uint8_t n){

    c->v[0xF] = 0;

    uint8_t draw_byte, screen_x, screen_y, sprite_bit, screen_bit;
    size_t location;

    for (int i = 0; i < n; i++)
    {
        screen_y = y + i;
        while(screen_y > 31)
            screen_y -= 32;

        draw_byte = c->memory[c->i+i];
        for (int k = 0; i < 8; k++)
        {
            sprite_bit = (draw_byte << k) & 0x80;
            screen_x = x + k;
            while (screen_x > 63)
                screen_x -= 64;
            
            location = screen_y*64 + screen_x;
            screen_bit = c->vram[location];

            if(screen_bit && sprite_bit)
                c->v[0xF] = 1;

            c->vram[location] = sprite_bit ^ screen_bit;
            
        }
        
    }
    
    
}

void Cpu_cycle(cpu* c){
    uint16_t opcode = (uint16_t) (c->memory[c->pc++] << 8) | c->memory[c->pc++];

    uint16_t  nnn =  opcode & 0x0FFF;
    uint8_t     n =  opcode & 0x000F;
    uint8_t     x = (opcode & 0x0F00) >> 8;
    uint8_t     y = (opcode & 0x00F0) >> 4;
    uint8_t    kk =  opcode & 0x00FF;

    switch ((opcode & 0xF000) >> 12) {
        case 0x0:
            switch (opcode & 0x00FF) {
                case 0xE0:
                    Cpu_CLS(c);
                    break;
                case 0xEE:
                    Cpu_RET(c);
                    break;
            }
            break;
        case 0x1:
            Cpu_JUMP(c, nnn);
            break;
        case 0x2:
            Cpu_CALL(c, nnn);
            break;
        case 0x3:
            Cpu_SE(c, x, kk);
            break;
        case 0x4:
            Cpu_SNE(c, x, kk);
            break;
        case 0x5:
            Cpu_SEV(c, x, y);
            break;
        case 0x6:
            Cpu_LD(c, x, kk);
            break;
        case 0x7:
            Cpu_ADD(c, x, kk);
            break;
        case 0x8:
            switch (n) {
                case 0x0:
                    Cpu_LDV(c, x, y);
                    break;
                case 0x1:
                    Cpu_OR(c, x, y);
                    break;
                case 0x2:
                    Cpu_AND(c, x, y);
                    break;
                case 0x3:
                    Cpu_XOR(c, x, y);
                    break;
                case 0x4:
                    Cpu_ADDC(c, x, y);
                    break;
                case 0x5:
                    Cpu_SUB(c, x, y);
                    break;
                case 0x6:
                    Cpu_SHR(c, x);
                    break;
                case 0x7:
                    Cpu_SUBN(c, x, y);
                    break;
                case 0xE:
                    Cpu_SHL(c, x);
                    break;
            }
            break;
        case 0x9:
            Cpu_SNEV(c, x, y);
            break;
        case 0xA:
            Cpu_LDI(c, nnn);
            break;
        case 0xB:
            Cpu_JUMPV(c, nnn);
            break;
        case 0xC:
            Cpu_RND(c, x, kk);
    }

}