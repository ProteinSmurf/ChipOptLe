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
//  An 8 bit sound timer which functions like the delay timer, but which also gives off a beep sound as long as it is not 0                             |
//  16 * 8 bit(one byte) general - purpose variable registers numbered 0 to F hexadecimal, or 0 to 15 in decimal, called V0 to VF                       |
//  VF is also used as a flag register; many instructions will set it to either 1 or 0 based on some rule, for example using it as a carry flag         |


//  Note: Opcode is 16 bit ( 2 byte ) long
class Chip
{
    //Variables - Data stored
private:
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t I;   // Depending on specification this is either 16 or 12 , will go with the first option
    uint16_t pc;
    uint16_t stack[16];
    int stackPointer;
    uint8_t display[64 * 32];

    uint8_t delay_timer;
    uint8_t sound_timer;
    bool needRedraw;

public:
    int keys[16];

    //Methods
public:
    void init();                                        //  Set everything to 0 to avoid possible unwanted behaviour and ProgramCounter to 0x200 as that is the starting point in memory for loaded ROM
    void run();                                         //  Fetch - Decode - Execute loop
    void loadProgram(const std::string& filePath);      //  Starting from 0x200 , load into memory data from ROM file
    uint8_t* getDisplay();

private:
    void printMemoryMap();
    void loadFontset();
    //opcodes
    void unsupportedOpcode();
    void opcode_00E0();
    void opcode_00EE();
    void opcode_1NNN(uint16_t opcode);
    void opcode_2NNN(uint16_t opcode);
    void opcode_3XNN(uint16_t opcode);
    void opcode_4XNN(uint16_t opcode);
    void opcode_5XY0(uint16_t opcode);
    void opcode_6XNN(uint16_t opcode);
    void opcode_7XNN(uint16_t opcode);
    void opcode_8XY0(uint16_t opcode);
    void opcode_8XY1(uint16_t opcode);
    void opcode_8XY2(uint16_t opcode);
    void opcode_8XY3(uint16_t opcode);
    void opcode_8XY4(uint16_t opcode);
    void opcode_8XY5(uint16_t opcode);
    void opcode_8XY6(uint16_t opcode);
    void opcode_8XY7(uint16_t opcode);
    void opcode_8XYE(uint16_t opcode);
    void opcode_9XY0(uint16_t opcode);
    void opcode_ANNN(uint16_t opcode);
    void opcode_BNNN(uint16_t opcode);
    void opcode_CXNN(uint16_t opcode);
    void opcode_DXYN(uint16_t opcode);
    void opcode_EX9E(uint16_t opcode);
    void opcode_EXA1(uint16_t opcode);
    void opcode_FX07(uint16_t opcode);
    void opcode_FX0A(uint16_t opcode);
    void opcode_FX15(uint16_t opcode);
    void opcode_FX18(uint16_t opcode);
    void opcode_FX1E(uint16_t opcode);
    void opcode_FX29(uint16_t opcode);
    void opcode_FX33(uint16_t opcode);
    void opcode_FX55(uint16_t opcode);
    void opcode_FX65(uint16_t opcode);
};

#endif // CHIP_H
