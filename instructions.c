#include "chip8_def.h"
/*
First Opcode: 00E0
CLS: Clear the display, set all pixels to 0
*/
void cls(Chip8* chip8) {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            chip8->screen[i][j] = 0;
        }
    }
    chip8->draw_screen_flag = TRUE;
    chip8->pc_reg += 2;
}
/*
00EE: RET returns from the subroutine
*/
void ret(Chip8* chip8) {
    chip8->sp_reg--;
    chip8->pc_reg = chip8->stack[chip8->sp_reg];
    chip8->pc_reg += 2;
}
/*
1nnn: JP addr Jump to location nnn
Set PC to nnn
1NNN: This is the opcode format. The 1 is the most significant nibble (4 bits) of the opcode, indicating that this is a jump instruction. The NNN part is the address to which the program should jump.

NNN: This is a 12-bit address, which can range from 0x000 to 0xFFF. This address is extracted from the opcode by masking the lower 12 bits.
*/
void jp_addr(Chip8* chip8) {
    uint16_t addr = chip8->current_op & 0x0FFF;
    chip8->pc_reg = addr;
}

/*
2nnn - CALL addr
Call subroutine at nnn.

The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
*/
void call_addr(Chip8* chip8) {
    chip8->stack[chip8->sp_reg] = chip8->pc_reg;
    chip8->sp_reg++;
    uint16_t addr = chip8->current_op & 0x0FFF;
    chip8->pc_reg = addr;
}
/*
3xkk - SE Vx, byte
Skip next instruction if Vx = kk.

The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
*/
void se_vx_byte(Chip8* chip8) {
    uint16_t targetVreg = (chip8->current_op & 0x0F00);
    targetVreg = targetVreg >> 8; // Isolate X which is the register index
    uint8_t kk = chip8->current_op & 0x00FF;

    if(chip8->V[targetVreg] == kk) {
        chip8->pc_reg += 4; //skips 2 instructions
    } else {
        chip8->pc_reg += 2;
    }
}

/*
4xkk - SNE Vx, byte
Skip next instruction if Vx != kk.

The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
*/
void sne_vx_byte(Chip8* chip8) {
    uint16_t targetVreg = (chip8->current_op & 0x0F00);
    targetVreg = targetVreg >> 8; // Isolate X which is the register index
    uint8_t kk = chip8->current_op & 0x00FF;

    if(chip8->V[targetVreg] != kk) {
        chip8->pc_reg += 4; //skips 2 instructions
    } else {
        chip8->pc_reg += 2;
    }
}
/*
5xy0 - SE Vx, Vy
Skip next instruction if Vx = Vy.

The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
*/
void se_vx_vy(Chip8* chip8) {
    uint16_t targetVregX = (chip8->current_op & 0x0F00);
    targetVregX = targetVregX >> 8; // Isolate X which is the register index
    uint16_t targetVregY = (chip8->current_op & 0x00F0);
    targetVregY = targetVregY >> 4; // Isolate Y which is the register index

    if(chip8->V[targetVregX] == chip8->V[targetVregY]) {
        chip8->pc_reg += 4; //skips 2 instructions
    } else {
        chip8->pc_reg += 2;
    }
}
/*
6xkk - LD Vx, byte
Set Vx = kk.

The interpreter puts the value kk into register Vx.
*/
void ld_vx_byte(Chip8* chip8) {
    uint16_t targetVreg = (chip8->current_op & 0x0F00);
    targetVreg = targetVreg >> 8; // Isolate X which is the register index
    uint8_t kk = chip8->current_op & 0x00FF;

    chip8->V[targetVreg] = kk;
    chip8->pc_reg += 2;
}
/*
7xkk - ADD Vx, byte
Set Vx = Vx + kk.

Adds the value kk to the value of register Vx, then stores the result in Vx.
 */
void add_vx(Chip8* chip8) {
    uint16_t targetVreg = (chip8->current_op & 0x0F00);
    targetVreg = targetVreg >> 8; // Isolate X which is the register index
    uint8_t kk = chip8->current_op & 0x00FF;

    chip8->V[targetVreg] += kk;
    chip8->pc_reg += 2;
}
/*
8xy0 - LD Vx, Vy
Set Vx = Vy.

Stores the value of register Vy in register Vx.
*/
void ld_vx_vy(Chip8* Chip8) {
    uint16_t targetVregX = (Chip8->current_op & 0x0F00);
    targetVregX = targetVregX >> 8; // Isolate X which is the register index
    uint16_t targetVregY = (Chip8->current_op & 0x00F0);
    targetVregY = targetVregY >> 4; // Isolate Y which is the register index

    Chip8->V[targetVregX] = Chip8->V[targetVregY];
    Chip8->pc_reg += 2;
}
/*
8xy1 - OR Vx, Vy
Set Vx = Vx OR Vy.

Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. 
A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. 
Otherwise, it is 0.
*/
void or_vx_vy(Chip8* Chip8) {
    uint16_t targetVregX = (Chip8->current_op & 0x0F00);
    targetVregX = targetVregX >> 8; // Isolate X which is the register index
    uint16_t targetVregY = (Chip8->current_op & 0x00F0);
    targetVregY = targetVregY >> 4; // Isolate Y which is the register index

    Chip8->V[targetVregX] |= Chip8->V[targetVregY];
    Chip8->pc_reg += 2;
}

/*
8xy2 - AND Vx, Vy
Set Vx = Vx AND Vy.

Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. 
A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1. 
Otherwise, it is 0.
*/
void and_vx_vy(Chip8* Chip8) {
    uint16_t targetVregX = (Chip8->current_op & 0x0F00);
    targetVregX = targetVregX >> 8; // Isolate X which is the register index
    uint16_t targetVregY = (Chip8->current_op & 0x00F0);
    targetVregY = targetVregY >> 4; // Isolate Y which is the register index

    Chip8->V[targetVregX] &= Chip8->V[targetVregY];
    Chip8->pc_reg += 2;
}
/*
8xy3 - XOR Vx, Vy
Set Vx = Vx XOR Vy.

Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. 
An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, 
then the corresponding bit in the result is set to 1. Otherwise, it is 0.
*/
void xor_vx_vy(Chip8* Chip8) {
    uint16_t targetVregX = (Chip8->current_op & 0x0F00);
    targetVregX = targetVregX >> 8; // Isolate X which is the register index
    uint16_t targetVregY = (Chip8->current_op & 0x00F0);
    targetVregY = targetVregY >> 4; // Isolate Y which is the register index

    Chip8->V[targetVregX] ^= Chip8->V[targetVregY];
    Chip8->pc_reg += 2;
}
/*
8xy4 - ADD Vx, Vy
Set Vx = Vx + Vy, set VF = carry.

The values of Vx and Vy are added together. 
If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. 
Only the lowest 8 bits of the result are kept, and stored in Vx.
*/
void add_Vx_Vy(Chip8 *chip8) {
    uint8_t target_v_reg_x = (chip8->current_op & 0x0F00) >> 8;
    uint8_t target_v_reg_y = (chip8->current_op & 0x00F0) >> 4;
    uint16_t sum = (chip8->V[target_v_reg_x] + chip8->V[target_v_reg_y]);

    if (sum > 255) {
        chip8->V[0xF] = 1;
    }
    else {
        chip8->V[0xF] = 0;
    }

    chip8->V[target_v_reg_x] = (sum & 0xFF);
    chip8->pc_reg += 2;
}
/*
8xy5 - SUB Vx, Vy
Set Vx = Vx - Vy, set VF = NOT borrow.

If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
*/
void sub_vx_vy(Chip8* chip8) {
    uint16_t targetVregX = (chip8->current_op & 0x0F00);
    targetVregX = targetVregX >> 8; // Isolate X which is the register index
    uint16_t targetVregY = (chip8->current_op & 0x00F0);
    targetVregY = targetVregY >> 4; // Isolate Y which is the register index

    if(chip8->V[targetVregX] > chip8->V[targetVregY]) {
        chip8->V[0xF] = 1;
    } else {
        chip8->V[0xF] = 0;
    }

    chip8->V[targetVregX] -= chip8->V[targetVregY];
    chip8->pc_reg += 2;
}
/*
8xy6 - SHR Vx {, Vy}
Set Vx = Vx SHR 1.

If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
*/
void shr_vx_vy(Chip8* chip8) {
    uint16_t targetVregX = (chip8->current_op & 0x0F00);
    targetVregX = targetVregX >> 8; // Isolate X which is the register index

    chip8->V[0xF] = chip8->V[targetVregX] & 0x1;
    chip8->V[targetVregX] >>= 1;
    chip8->pc_reg += 2;
}
/*
8xy7 - SUBN Vx, Vy
Set Vx = Vy - Vx, set VF = NOT borrow.

If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
*/
void subn_vx_vy(Chip8* chip8) {
    uint16_t targetVregX = (chip8->current_op & 0x0F00);
    targetVregX = targetVregX >> 8; // Isolate X which is the register index
    uint16_t targetVregY = (chip8->current_op & 0x00F0);
    targetVregY = targetVregY >> 4; // Isolate Y which is the register index

    if(chip8->V[targetVregY] > chip8->V[targetVregX]) {
        chip8->V[0xF] = 1;
    } else {
        chip8->V[0xF] = 0;
    }

    chip8->V[targetVregX] = chip8->V[targetVregY] - chip8->V[targetVregX];
    chip8->pc_reg += 2;
}
/*
8xyE - SHL Vx {, Vy}
Set Vx = Vx SHL 1.

If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
*/
void shl_vx_vy(Chip8* chip8) {
    uint16_t targetVregX = (chip8->current_op & 0x0F00);
    targetVregX = targetVregX >> 8; // Isolate X which is the register index

    chip8->V[0xF] = chip8->V[targetVregX] >> 7;
    chip8->V[targetVregX] <<= 1;
    chip8->pc_reg += 2;
}
/*
9xy0 - SNE Vx, Vy
Skip next instruction if Vx != Vy.

The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
*/
void sne_vx_vy(Chip8* chip8) {
    uint16_t targetVregX = (chip8->current_op & 0x0F00);
    targetVregX = targetVregX >> 8; // Isolate X which is the register index
    uint16_t targetVregY = (chip8->current_op & 0x00F0);
    targetVregY = targetVregY >> 4; // Isolate Y which is the register index

    if(chip8->V[targetVregX] != chip8->V[targetVregY]) {
        chip8->pc_reg += 4; //skips 2 instructions
    } else {
        chip8->pc_reg += 2;
    }
}
/*
Annn - LD I, addr
Set I = nnn.

The value of register I is set to nnn.
*/
void ld_i_addr(Chip8* chip8) {
    uint16_t addr = chip8->current_op & 0x0FFF;
    chip8->I_reg = addr;
    chip8->pc_reg += 2;
}
/*
Bnnn - JP V0, addr
Jump to location nnn + V0.

The program counter is set to nnn plus the value of V0.
*/
void jp_v0_addr(Chip8* chip8) {
    uint16_t addr = chip8->current_op & 0x0FFF;
    chip8->pc_reg = addr + chip8->V[0];
}
/*
Cxkk - RND Vx, byte
Set Vx = random byte AND kk.

The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx.
*/
void rnd(Chip8 *chip8) {
    uint8_t target_v_reg = (chip8->current_op & 0x0F00) >> 8;
    uint8_t kk = chip8->current_op & 0x00FF;
    uint8_t random_num = rand() % 256;

    chip8->V[target_v_reg] = random_num & kk;
    chip8->pc_reg += 2;
}
/*
Dxyn - DRW Vx, Vy, nibble
Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

The interpreter reads n bytes from memory, starting at the address stored in I. 
These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). 
Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. 
If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen.
*/
void drw(Chip8 *chip8) {
    uint8_t target_v_reg_x = (chip8->current_op & 0x0F00) >> 8;
    uint8_t target_v_reg_y = (chip8->current_op & 0x00F0) >> 4;
    uint8_t sprite_height = chip8->current_op & 0x000F;
    uint8_t x_location = chip8->V[target_v_reg_x];
    uint8_t y_location = chip8->V[target_v_reg_y];
    uint8_t pixel;

    // Reset collision register to FALSE
    chip8->V[0xF] = FALSE;
    for (int y_coordinate = 0; y_coordinate < sprite_height; y_coordinate++) {
        pixel = chip8->ram[chip8->I_reg + y_coordinate];
        for (int x_coordinate = 0; x_coordinate < 8; x_coordinate++) {
            if ((pixel & (0x80 >> x_coordinate)) != 0) {
                if (chip8->screen[y_location + y_coordinate][x_location + x_coordinate] == 1) {
                    chip8->V[0xF] = TRUE;
                }
                chip8->screen[y_location + y_coordinate][x_location + x_coordinate] ^= 1;
            }
        }
    }

    chip8->draw_screen_flag = TRUE;
    chip8->pc_reg += 2;
}

/*
Ex9E - SKP Vx
Skip next instruction if key with the value of Vx is pressed.

Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
*/
void skp_vx(Chip8* chip8) {
    uint8_t targetVreg = (chip8->current_op & 0x0F00);
    targetVreg = targetVreg >> 8; // Isolate X which is the register index

    if(chip8->keyboard[chip8->V[targetVreg]] == TRUE) {
        chip8->pc_reg += 4; //skips 2 instructions
    } else {
        chip8->pc_reg += 2;
    }
}
/*
ExA1 - SKNP Vx
Skip next instruction if key with the value of Vx is not pressed.

Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
*/
void sknp_vx(Chip8* chip8) {
    uint8_t targetVreg = (chip8->current_op & 0x0F00);
    targetVreg = targetVreg >> 8; // Isolate X which is the register index

    if(chip8->keyboard[chip8->V[targetVreg]] == FALSE) {
        chip8->pc_reg += 4; //skips 2 instructions
    } else {
        chip8->pc_reg += 2;
    }
}
/*
Fx07 - LD Vx, DT
Set Vx = delay timer value.

The value of DT is placed into Vx.
*/
void ld_vx_dt(Chip8* chip8) {
    uint8_t targetVreg = (chip8->current_op & 0x0F00);
    targetVreg = targetVreg >> 8; // Isolate X which is the register index

    chip8->V[targetVreg] = chip8->delay_timer;
    chip8->pc_reg += 2;
}
/*
* Opcode FX0A: Load Vx, K
* Waits for a key press
* V[X] set to value of key (K) pressed
*/
void ld_Vx_k(Chip8 *chip8) {
    uint8_t target_v_reg = (chip8->current_op & 0x0F00) >> 8;

    chip8->was_key_pressed = FALSE;

    for (int i = 0; i < NUM_KEYS; i++) {
        if (chip8->keyboard[i] != FALSE) {
            chip8->V[target_v_reg] = i;
            chip8->was_key_pressed = TRUE;
        }
    }

    if (!chip8->was_key_pressed) {
        return;
    }

    chip8->pc_reg += 2;
}

/*
Fx15 - LD DT, Vx
Set delay timer = Vx.

DT is set equal to the value of Vx.
*/
void ld_dt_vx(Chip8* chip8) {
    uint8_t targetVreg = (chip8->current_op & 0x0F00);
    targetVreg = targetVreg >> 8; // Isolate X which is the register index

    chip8->delay_timer = chip8->V[targetVreg];
    chip8->pc_reg += 2;
}

/*
Fx18 - LD ST, Vx
Set sound timer = Vx.

ST is set equal to the value of Vx.
*/
void ld_st_vx(Chip8* chip8) {
    uint8_t targetVreg = (chip8->current_op & 0x0F00);
    targetVreg = targetVreg >> 8; // Isolate X which is the register index

    chip8->sound_timer = chip8->V[targetVreg];
    chip8->pc_reg += 2;
}
/*
Fx1E - ADD I, Vx
Set I = I + Vx.

The values of I and Vx are added, and the results are stored in I.
*/
void add_i_vx(Chip8* chip8) {
    uint8_t targetVreg = (chip8->current_op & 0x0F00);
    targetVreg = targetVreg >> 8; // Isolate X which is the register index

    chip8->I_reg += chip8->V[targetVreg];
    chip8->pc_reg += 2;
}
/*
Fx29 - LD F, Vx
Set I = location of sprite for digit Vx.

The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
*/
void ld_F_Vx(Chip8 *chip8) {
    uint8_t target_v_reg = (chip8->current_op & 0x0F00) >> 8;

    chip8->I_reg = (chip8->V[target_v_reg] * 0x5);
    chip8->pc_reg += 2;
}
/*
Fx33 - LD B, Vx
Store BCD representation of Vx in memory locations I, I+1, and I+2.

The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
*/
void st_bcd_Vx(Chip8 *chip8) {
    uint8_t target_v_reg = (chip8->current_op & 0x0F00) >> 8;

    chip8->ram[chip8->I_reg] = chip8->V[target_v_reg] / 100;                 // MSb
    chip8->ram[chip8->I_reg + 1] = (chip8->V[target_v_reg] / 10) % 10;
    chip8->ram[chip8->I_reg + 2] = (chip8->V[target_v_reg] % 100) % 10;      // LSb
    chip8->pc_reg += 2;
}
/*
Fx55 - LD [I], Vx
Store registers V0 through Vx in memory starting at location I.

The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
*/
void st_V_regs(Chip8 *chip8) {
    uint8_t end_ld_v_reg = (chip8->current_op & 0x0F00) >> 8;

    for (int i = 0; i <= end_ld_v_reg; i++) {
        chip8->ram[chip8->I_reg + i] = chip8->V[i];
    }

    // TODO: Does I_reg need to change?
    chip8->I_reg += (end_ld_v_reg + 1);

    chip8->pc_reg += 2;
}
/*
* Opcode FX65: LD Vx, I
* Read values into V[0] - V[X] from memory starting at I_reg value
*/
void ld_V_regs(Chip8 *chip8) {
    uint8_t end_ld_v_reg = (chip8->current_op & 0x0F00) >> 8;

    for (int i = 0; i <= end_ld_v_reg; i++) {
        chip8->V[i] = chip8->ram[chip8->I_reg + i];
    }

    // TODO: Does I_reg need to change?
    chip8->I_reg += (end_ld_v_reg + 1);

    chip8->pc_reg += 2;
}
// DONE WITH CODING THE INSTRUCTIONS NOT SURE IF THEY ARE CODED CORRECTLY
