#include "/opt/homebrew/Cellar/sdl2/2.30.3/include/SDL2/SDL.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

typedef struct Chip8 {
    uint8_t memory[4096];
    uint8_t registers[16];
    uint16_t index;
    uint16_t pc;
    uint16_t stack[16];
    uint8_t sp; //stack pointer
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t keypad[16];
    uint32_t video[64 * 32];
    uint16_t opcode;
} Chip8;

void init(Chip8* chip8) {
    //clear memory
    memset(chip8->memory, 0, sizeof(chip8->memory));
    //clear registers stack keypad and vid memory
    memset(chip8->registers, 0, sizeof(chip8->registers));
    memset(chip8->stack, 0, sizeof(chip8->stack));
    memset(chip8->keypad, 0, sizeof(chip8->keypad));
    memset(chip8->video, 0, sizeof(chip8->video));
    //intialize special registers
    chip8->index = 0;
    chip8->pc = 0x200;
    chip8->sp = 0;
    chip8->opcode = 0;
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
    //loading fontset
    uint8_t fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for (int i = 0; i < 80; i++) {
        chip8->memory[i] = fontset[i];
    }
    //reset timers
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
}

void loadROM(Chip8* chip8, const char* filename) {
    // Open the ROM file
    FILE* rom = fopen(filename, "rb");
    if (rom == NULL) {
        fprintf(stderr, "Failed to open ROM: %s\n", filename);
        exit(1);
    }

    // Find the size of the ROM
    fseek(rom, 0, SEEK_END);
    long rom_size = ftell(rom);
    rewind(rom);

    // Read the ROM into a buffer
    uint8_t buffer[rom_size];
    size_t result = fread(buffer, sizeof(uint8_t), rom_size, rom);
    if (result != rom_size) {
        fprintf(stderr, "Failed to read ROM\n");
        exit(1);
    }

    // Copy the buffer into the CHIP-8 memory starting at 0x200
    for (int i = 0; i < rom_size; ++i) {
        chip8->memory[i + 512] = buffer[i];
    }

    // Close the ROM file
    fclose(rom);
}

void emulateOneCycle(Chip8* Chip8) {
    //fetch opcode from memory
    Chip8->opcode = Chip8->memory[Chip8->pc] << 8 | Chip8->memory[Chip8->pc + 1];
    switch (Chip8->opcode & 0xF000) {
        
    }
}





