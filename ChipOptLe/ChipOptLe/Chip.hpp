#ifndef CHIP_H
#define CHIP_H

#include <iostream>




//                  CHIP8 - SPECIFICATIONS  
//  Memory: 4 kilobytes of RAM                                                                                                                          |
//  Display : 64 x 32 pixels(or 128 x 64 for SUPER - CHIP) |  by default: black or white                                                                |
//  A program counter, often called just “PC”, which points at the current instruction in memory                                                        |       
//  One 16 bit index register called “I” which is used to point at locations in memory                                                                  |
//  A stack for 16 bit addresses, which is used to call subroutines / functions and return from them                                                    |
//  An 8 bit delay timer which is decremented at a rate of 60 Hz(60 times per second).                                                                  |
//  An 8 bit sound timer which functions like the delay timer, but which also gives off a beep sound as long as it is not 0                              |
//  16 * 8 bit(one byte) general - purpose variable registers numbered 0 to F hexadecimal, or 0 to 15 in decimal, called V0 to VF                       |
//  VF is also used as a flag register; many instructions will set it to either 1 or 0 based on some rule, for example using it as a carry flag         |


//  Note: Opcode is 16 bit ( 2 byte ) long
class Chip
{
    //Variables - Data stored
private:
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t I;
    uint16_t pc;
    uint16_t stack[16];
    int stackPointer;
    uint8_t display[64 * 32];

    uint8_t delay_timer;
    uint8_t sound_timer;




public:
    int keys[16];

    //Methods
public:
    void init();                                        //  Set everything to 0 to avoid possible unwanted behaviour and ProgramCounter to 0x200 as that is the starting point in memory for loaded ROM
    void run();                                         //  Fetch - Decode - Execute loop
    void loadProgram(const std::string& filePath);      //  Starting from 0x200 , load into memory data from ROM file

private:
    //opcodes
    void unsupportedOpcode();
    void opcode_00E0();
    void opcode_00EE();
    void opcode_1NNN(unsigned short opcode);
    void opcode_2NNN(unsigned short opcode);
    void opcode_3XNN(unsigned short opcode);
    void opcode_4XNN(unsigned short opcode);
    void opcode_5XY0(unsigned short opcode);
    void opcode_6XNN(unsigned short opcode);
    void opcode_7XNN(unsigned short opcode);
    void opcode_8XY0(unsigned short opcode);
    void opcode_8XY1(unsigned short opcode);
    void opcode_8XY2(unsigned short opcode);
    void opcode_8XY3(unsigned short opcode);
    void opcode_8XY4(unsigned short opcode);
    void opcode_8XY5(unsigned short opcode);
    void opcode_8XY6(unsigned short opcode);
    void opcode_8XY7(unsigned short opcode);
    void opcode_8XYE(unsigned short opcode);
    void opcode_9XY0(unsigned short opcode);
    void opcode_ANNN(unsigned short opcode);
    void opcode_BNNN(unsigned short opcode);
    void opcode_CXNN(unsigned short opcode);
    void opcode_DXYN(unsigned short opcode);
    void opcode_EX9E(unsigned short opcode);
    void opcode_EXA1(unsigned short opcode);
    void opcode_FX07(unsigned short opcode);
    void opcode_FX0A(unsigned short opcode);
    void opcode_FX15(unsigned short opcode);
    void opcode_FX18(unsigned short opcode);
    void opcode_FX1E(unsigned short opcode);
    void opcode_FX29(unsigned short opcode);
    void opcode_FX33(unsigned short opcode);
    void opcode_FX55(unsigned short opcode);
    void opcode_FX65(unsigned short opcode);
};

#endif // CHIP_H
