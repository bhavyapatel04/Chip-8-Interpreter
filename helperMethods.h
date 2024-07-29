#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "instructions.h"
#include "chip8_def.h"

void initialize(Chip8* chip8) {
    // Program counter starts at 0x200
    chip8->pc_reg = PC_START;
    chip8->current_op = 0;
    chip8->I_reg = 0;
    chip8->sp_reg = 0;

    // Clear display
    memset(chip8->screen, 0, sizeof(chip8->screen));
    // Clear stack
    memset(chip8->stack, 0, sizeof(chip8->stack));
    // Clear registers V0-VF
    memset(chip8->V, 0, sizeof(chip8->V));
    // Clear memory
    memset(chip8->ram, 0, sizeof(chip8->ram));

    // Load fontset into memory
    for(int i = 0; i < FONTSET_SIZE; i++) {
        chip8->ram[i] = FONTSET[i];
    }

    // Reset timers
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;

    // Set status flags
    chip8->is_running_flag = TRUE;
    chip8->draw_screen_flag = FALSE;
    chip8->is_paused_flag = FALSE;

    // Keyboard setup, Clear all keys
    for (int i = 0; i < NUM_KEYS; i++) {
        chip8->keyboard[i] = FALSE;
    }
    chip8->was_key_pressed = FALSE;
}

// Largely similar to the init function, however all of the ram is not cleared 
// (so the rom does not have to be re-loaded into memory)
void reset_system(Chip8 *chip8) {
    chip8->is_running_flag = TRUE;
    chip8->draw_screen_flag = FALSE;
    chip8->is_paused_flag = FALSE;

    chip8->pc_reg = PC_START;
    chip8->current_op = 0;
    chip8->sp_reg = 0;
    chip8->I_reg = 0;

    // Clear display (memory)
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            chip8->screen[i][j] = 0;
        }
    }

    // Clear ram from the fontset end (80) to the Program ram 
    for (int i = 80; i < PROGRAM_START_ADDR; i++) {
        chip8->ram[i] = 0;
    }

    // Clear registers, keyboard and stack (all 16 each)
    for (int i = 0; i < 16; i++) {
        chip8->V[i] = 0;
        chip8->keyboard[i] = FALSE;
        chip8->stack[i] = 0;
    }
    // Reset timers to 0
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
}


