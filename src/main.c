#include <stdint.h>
#include <stdio.h>
#include "SDL_rect.h"
#include "SDL_render.h"
#include "cpu.h"
#include <inttypes.h>
#include "SDL.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320

void testSkips();
void loadMemory(cpu* c, uint16_t opcodes[], uint16_t size);

int main() {

    cpu c = Cpu_new();

    uint16_t opcodes[] = {
            0xA032,
            0x603C,
            0x611B,
            0xD015,
            0xA000
    };


    loadMemory(&c, opcodes, (sizeof opcodes)/(sizeof opcodes[0]));

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(640, 320, 0, &window, &renderer);

    int cycles = 0;
    while (c.pc < 0x20A){
        Cpu_cycle(&c);
        cycles++;
    }
    
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );

    // Clear winow
    SDL_RenderClear( renderer );

    int scaleX = WINDOW_WIDTH / 64;
    int scaleY = WINDOW_HEIGHT / 32;

    SDL_Rect r;
    r.x = 50;
    r.y = 50;
    r.w = scaleX;
    r.h = scaleY;

    // Set render color to blue ( rect will be rendered in this color )
    SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );

    uint8_t draw;

    for(int i=0; i<32; i++){
        for(int j=0; j<64; j++){

            draw = c.vram[(i*64)+j];

            if(draw){

                r.x=j*scaleX;
                r.y=i*scaleY;

                SDL_RenderFillRect(renderer, &r);
            }

        }
    }

    SDL_RenderPresent(renderer);
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    //Cpu_dumpVRAM(&c);

    return 0;
}

void loadMemory(cpu* c, uint16_t opcodes[], uint16_t size){
    for (int i=0, j = 0x200; i < size; ++i) {
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
