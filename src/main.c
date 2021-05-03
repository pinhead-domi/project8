#include <stdio.h>
#include "cpu.h"
#include <inttypes.h>

void testSkips();
void loadMemory(cpu* c, uint16_t opcodes[], uint16_t size);

int main() {

    cpu c = Cpu_new();

    uint16_t opcodes[] = {
            0x6002,
            0x8006
    };

    loadMemory(&c, opcodes, (sizeof opcodes)/(sizeof opcodes[0]));

    int cycles = 0;
    while (c.pc < 4){
        Cpu_cycle(&c);
        cycles++;
    }

    printf("Cpu completed program using %d cylces, yeah!\n", cycles);

    return 0;
}

void loadMemory(cpu* c, uint16_t opcodes[], uint16_t size){
    for (int i=0, j = 0; i < size; ++i) {
        c->memory[j++] = opcodes[i] >> 8;
        c->memory[j++]   = opcodes[i];
    }
}

void testSkips(){
    cpu c = Cpu_new();

    c.memory[0]=0x60;
    c.memory[1]=0xab;

    c.memory[2]=0x61;
    c.memory[3]=0xab;

    c.memory[4]=0x71;
    c.memory[5]=0x01;

    c.memory[6]=0x50;
    c.memory[7]=0x10;

    c.memory[8]=0x20;
    c.memory[9]=0xFF;

    for (int i = 0; i < 5; i++) {
        Cpu_cycle(&c);
    }
}

void testFunctionCall(){
    cpu c = Cpu_new();

    //0x100F -> Jump to 0x00F
    c.memory[0]=0x10;
    c.memory[1]=0x0F;

    //0x20FF -> Call function at 0x0FF
    c.memory[0x00F]=0x20;
    c.memory[0x00F+1]=0xFF;

    //0x00EE -> Return from function
    c.memory[0xFF]=0x00;
    c.memory[0xFF+1]=0xEE;

    Cpu_cycle(&c);
    Cpu_cycle(&c);
    Cpu_cycle(&c);
}
