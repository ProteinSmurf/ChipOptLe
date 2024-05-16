#include "Chip.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "ChipFontset.h"


//Initialize everything to avoid possible unwanted behaviour
void Chip::init()
{
    memset(memory, 0, sizeof(memory));
    memset(V, 0, sizeof(V));
    I = 0x0;
    pc = 0x200;

    memset(stack, 0, sizeof(stack));
    stackPointer = 0;

    memset(display, 0, sizeof(display));

    delay_timer = 0;
    sound_timer = 0;
}


void Chip::run()
{
    // Fetch Opcode - Opcode is 2 byte ( 16 bit ) 
    // if Memory[0x200] = 0xAB and Memory[0x201] = 0xCD
    // Opcode = 0xAB00 | 0x00CD = 0xABCD

    uint16_t opcode = (memory[pc] << 8) | memory[pc + 1];
    std::cout << std::hex << std::uppercase << opcode << ": ";

    //Decode opcode + execute when done decoding
    switch (opcode & 0xF000)
    {
    case 0x0000: //Multi-case
    {
        switch (opcode & 0x00FF)
        {
            case 0x00E0: //00E0: Clear Screen
            {
                opcode_00E0();
                break;
            }
            case 0x00EE: //00EE: Returns from subroutine
            {
                opcode_00EE();
                break;
            }
            default: //0NNN: Calls RCA 1802 Program at address NNN ( NOT USED IN MODERN IMPLEMENTATIONS )
            {
                unsupportedOpcode();
                break;
            }
        }
        break; // Added this break to match the opening brace
    }
    case 0x1000: //1NNN: Jumps to address NNN
    {
        opcode_1NNN(opcode);
        break;
    }
    case 0x2000: //2NNN: Calls subroutine at NNN
    {
        opcode_2NNN(opcode);
        break;
    }
    case 0x3000: //3XNN: Skips the next instruction if VX equals NN
    {
        opcode_3XNN(opcode);
        break;
    }
    case 0x4000: //4XNN: Skip the next instruction if VX != NN
    {
        opcode_4XNN(opcode);
        break;
    }
    case 0x5000: //5XY0 Skips the next instruction if VX equals VY.
    {
        opcode_5XY0(opcode);
        break;
    }
    case 0x6000: //6XNN: Set VX to NN
    {
        opcode_6XNN(opcode);
        break;
    }
    case 0x7000: //7XNN: Adds NN to VX
    {
        opcode_6XNN(opcode);
        break;
    }
    case 0x8000: //Contains more data in last nibble
    {
        switch (opcode & 0x000F)
        {
            case 0x0000: //8XY0: Sets VX to the value of VY
            {
                opcode_8XY0(opcode);
                break;
            }
            case 0x0001: //8XY1 Sets VX to VX or VY.
            {
                opcode_8XY1(opcode);
                break;
            }
            case 0x0002: //8XY2: Sets VX to VX AND VY
            {
                opcode_8XY2(opcode);
                break;
            }
            case 0x0003: //8XY3: Sets VX to VX xor VY.[13]
            {
                opcode_8XY3(opcode);
                break;
            }
            case 0x0004: //8XY4: Adds VY to VX. VF is set to 1 when there's an overflow, and to 0 when there is not.
            {
                opcode_8XY4(opcode);
                break;
            }
            case 0x0005: //VY is subtracted from VX. VF is set to 0 when there is a borrow else 1
            {
                opcode_8XY5(opcode);
                break;
            }
            case 0x0006: //8XY6: Shift VX right by one, VF is set to the least significant bit of VX
            {
                opcode_8XY6(opcode);
                break;
            }
            case 0x0007: //8XY7 Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
            {
                opcode_8XY7(opcode);
                break;
            }
            case 0x000E: //8XYE Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.
            {
                opcode_8XYE(opcode);
                break;
            }
            default:
            {
                unsupportedOpcode();
                break;
            }
        }
        break; // Added this break to match the opening brace
    }
    case 0x9000:
    {
        opcode_9XY0(opcode);
        break;
    }
    case 0xA000: //ANNN: Set I to NNN
    {
        opcode_ANNN(opcode);
        break;
    }
    case 0xB000: //BNNN Jumps to the address NNN plus V0.
    {
        opcode_BNNN(opcode);
        break;
    }
    case 0xC000: //CXNN: Set VX to a random number and NN
    {
        opcode_CXNN(opcode);
        break;
    }
    case 0xD000: //DXYN: Draw a sprite (X, Y) size (8, N). Sprite is located at I
    {
        opcode_DXYN(opcode);
        break;
    }
    case 0xE000:
    {
        switch (opcode & 0x00FF)
        {
            case 0x009E: //EX9E Skip the next instruction if the Key VX is pressed
            {
                opcode_EX9E(opcode);
                break;
            }
            case 0x00A1: //EXA1 Skip the next instruction if the Key VX is NOT pressed
            {
                opcode_EXA1(opcode);
                break;
            }
            default:
            {
                unsupportedOpcode();
                break;
            }
        }
        break; // Added this break to match the opening brace
    }
    case 0xF000:
    {
        switch (opcode & 0x00FF)
        {
            case 0x0007: //FX07: Set VX to the value of delay_timer
            {
                opcode_FX07(opcode);
                break;
            }
            case 0x000A: //FX0A A key press is awaited, and then stored in VX.
            {
                opcode_FX0A(opcode);
                break;
            }
            case 0x0015:  //FX15: Set delay timer to V[x]
            {
                opcode_FX15(opcode);
                break;
            }
            case 0x0018:  //FX18: Set the sound timer to V[x]
            {
                opcode_FX18(opcode);
                break;
            }
            case 0x001E:  //FX1E: Adds VX to I
            {
                opcode_FX1E(opcode);
                break;
            }
            case 0x0029:  //FX29: Sets I to the location of the sprite for the character VX (Fontset)
            {
                opcode_FX29(opcode);
                break;
            }
            case 0x0033:  //FX33 Store a binary-coded decimal value VX in I, I + 1 and I + 2
            {
                opcode_FX33(opcode);
                break;
            }
            case 0x0055:  //FX55 Stores V0 to VX in memory starting at address I.
            {
                opcode_FX55(opcode);
                break;
            }
            case 0x0065:  //FX65 Fills V0 to VX with values from I
            {
                opcode_FX65(opcode);
                break;
            }
            default:
            {
                unsupportedOpcode();
                break;
            }
        }
        break; // Added this break to match the opening brace
    }
    default:
    {
        unsupportedOpcode();
        break;
    }
    }
}


//========================================================================================================================//
//================================================ Opcodes  ==============================================================//
//========================================================================================================================//

void Chip::unsupportedOpcode()
{
    // Unsupported Opcode
    std::cerr << "Unsupported Opcode!" << std::endl;
    exit(0);
}

void Chip::opcode_00E0()
{
    // Clear Screen
    for (int i = 0; i < sizeof(display); i++)
    {
        display[i] = 0;
    }
    pc += 2;
}

void Chip::opcode_00EE()
{
    // Returns from subroutine

    stackPointer--;
    pc = stack[stackPointer];
    std::cout << "Returning to " << std::hex << std::uppercase << pc << std::endl;
}

void Chip::opcode_1NNN(uint16_t opcode)
{
    //1NNN: Jumps to address NNN
    uint16_t nnn = opcode & 0x0FFF;
    pc = nnn;
    std::cout << "Jumping to " << std::hex << std::uppercase << pc << std::endl;
}

void Chip::opcode_2NNN(uint16_t opcode)
{   
    //2NNN: Calls subroutine at NNN
    
    // stack is a LIFO(last in, first out)
    // Example for opcode behaviour to help understand: 
    // pc = 0x200  ( current instruction in memory )
    // stack[0] = pc ( 0x200)   | Store return address to stack[0] 
    // stackpointer increment 
    // pc = opcode & 0xFFF ( NNN )
    // so basically Save current instruction from memory  to stack , PC go to nnn 
    // ( do whatever untill returning from subroutine , in which case stack decrements and
    // PC = original position )

    uint16_t nnn = opcode & 0x0FFF;

    stack[stackPointer] = pc;
    stackPointer++;
    pc = nnn;
    std::cout << "Calling " << std::hex << std::uppercase << pc << " from " << std::hex << std::uppercase << stack[stackPointer - 1] << std::endl;
}

void Chip::opcode_3XNN(uint16_t opcode)
{
    //3XNN: Skips the next instruction if VX equals NN
    uint16_t x = (opcode & 0x0F00) >> 8; // shift right to become Right most nibble as I need the number itself
    uint16_t nn = opcode & 0x00FF;

    if (V[x] == nn)
    {
        pc += 4;
        std::cout << "Skipping next instruction (V[" << x << "] == " << nn << ")" << std::endl;
    }
    else
    {
        pc += 2;
        std::cout << "Not skipping next instruction (V[" << x << "] =/= " << nn << ")" << std::endl;
    }
}

void Chip::opcode_4XNN(uint16_t opcode)
{
    uint16_t x = (opcode & 0x0F00) >> 8;  // shift right to become Right most nibble as I need the number itself
    uint16_t nn = opcode & 0x00FF;
    if (V[x] != nn)
    {
        std::cout << "Skipping next instruction V[" << x << "] = " << V[x] << " != " << nn << std::endl;
        pc += 4;
    }
    else
    {
        std::cout << "Not skipping next instruction V[" << x << "] = " << V[x] << " == " << nn << std::endl;
        pc += 2;
    }
}

void Chip::opcode_5XY0(uint16_t opcode)
{
    uint16_t x = (opcode & 0x0F00) >> 8;  //same as 4XNN
    uint16_t y = (opcode & 0x00F0) >> 4;  // here same as X but less bits to shift for Y to become rightm ost nibble
    if (V[x] == V[y])
    {
        std::cout << "Skipping next instruction V[" << x << "] == V[" << y << "]" << std::endl;
        pc += 4;
    }
    else
    {
        std::cout << "Skipping next instruction V[" << x << "] =/= V[" << y << "]" << std::endl;
        pc += 2;
    }
}

void Chip::opcode_6XNN(uint16_t opcode)
{
    // 6XNN: Set VX to NN
    uint16_t x = (opcode & 0x0F00) >> 8;
    V[x] = opcode & 0x00FF;
    pc += 2;
    std::cout << "Setting V[" << x << "] to " << (int)V[x] << std::endl;
}


void Chip::opcode_7XNN(uint16_t opcode)
{
    //7XNN: Adds NN to VX
    uint16_t x = (opcode & 0x0F00) >> 8;
    uint16_t nn = opcode & 0x00FF;
    V[x] += nn;
    pc += 2;
    std::cout << "Adding " << nn << " to V[" << x << "] = " << (int)V[x] << std::endl;
}

void Chip::opcode_8XY0(uint16_t opcode)
{
    //8XY0: Sets VX to the value of VY
    uint16_t x = (opcode & 0x0F00) >> 8;
    uint16_t y = (opcode & 0x00F0) >> 4;
    std::cout << "Setting V[" << x << "] to " << (V[y]) << std::endl;
    V[x] = V[y];
    pc += 2;
}

void Chip::opcode_8XY1(uint16_t opcode)
{
    //8XY1 Sets VX to VX or VY.
    uint16_t x = (opcode & 0x0F00) >> 8;
    uint16_t y = (opcode & 0x00F0) >> 4;
    std::cout << "Setting V[" << x << "] = V[" << x << "] | V[" << y << "]" << std::endl;
    V[x] = (V[x] | V[y]) & 0xFF;
    pc += 2;
}

void Chip::opcode_8XY2(uint16_t opcode)
{
    //8XY2: Sets VX to VX AND VY
    uint16_t x = (opcode & 0x0F00) >> 8;
    uint16_t y = (opcode & 0x00F0) >> 4;
    std::cout << "Set V[" << x << "] to V[" << x << "] = " << (V[x]) << " & V[" << y << "] = " << (V[y]) << " = " << (V[x] & V[y]) << std::endl;
    V[x] = V[x] & V[y];
    pc += 2;
}

void Chip::opcode_8XY3(uint16_t opcode)
{
    //8XY3: Sets VX to VX xor VY.[13]
    uint16_t x = (opcode & 0x0F00) >> 8;
    uint16_t y = (opcode & 0x00F0) >> 4;
    std::cout << "Setting V[" << x << "] = V[" << x << "] ^ V[" << y << "]" << std::endl;
    V[x] = (V[x] ^ V[y]) & 0xFF;
    pc += 2;
}

void Chip::opcode_8XY4(uint16_t opcode)
{
    //8XY4: Adds VY to VX. VF is set to 1 when there's an overflow, and to 0 when there is not.
    //Add VY to VX. VF is set to 1 when carry applies else to 0
    uint16_t x = (opcode & 0x0F00) >> 8;
    uint16_t y = (opcode & 0x00F0) >> 4;
    std::cout << "Adding V[" << x << "] (" << (V[x]) << ") to V[" << y << "] (" << (V[y]) << ") = " << ((V[x] + V[y]) & 0xFF) << ", ";
    if (V[y] > 0xFF - V[x])   // V[y] > 0xFF(max value) - V[x] | Ex to understand:    k > 10 - 5  -> if k > 5 , carry
    {
        V[0xF] = 1;
        std::cout << "Carry!" << std::endl;
    }
    else
    {
        V[0xF] = 0;
        std::cout << "No Carry" << std::endl;
    }
    V[x] = (V[x] + V[y]) & 0xFF;
    pc += 2;
}

void Chip::opcode_8XY5(uint16_t opcode)
{
    //8XY5: VY is subtracted from VX. VF is set to 0 when there's an underflow, 
    // and 1 when there is not. (i.e. VF set to 1 if VX >= VY and 0 if not)

    uint16_t x = (opcode & 0x0F00) >> 8;
    uint16_t y = (opcode & 0x00F0) >> 4;
    std::cout << "V[" << x << "] = " << (V[x]) << " V[" << y << "] = " << (V[y]) << ", ";
    if (V[x] > V[y])
    {
        V[0xF] = 1;
        std::cout << "No Borrow" << std::endl;
    }
    else
    {
        V[0xF] = 0;
        std::cout << "Borrow" << std::endl;
    }
    V[x] = (V[x] - V[y]) & 0xFF;   
    pc += 2;
}

void Chip::opcode_8XY6(uint16_t opcode)
{   

    //8XY6: Shift VX right by one, VF is set to the least significant bit of VX
    //   Least Significant Bit (LSB) : The rightmost bit in a binary number.  2^0
    //   Most Significant Bit  (MSB) : The leftmost bit in a binary number.


    uint16_t x = (opcode & 0x0F00) >> 8;
    V[0xF] = V[x] & 0x1;   //VF = least significant bit of VX
    V[x] = V[x] >> 1;
    pc += 2;
    std::cout << "Shift V[ " << x << "] >> 1 and VF to LSB of VX" << std::endl;
}

void Chip::opcode_8XY7(uint16_t opcode)
{
    //8XY7: Sets VX to VY minus VX. VF is set to 0 when there's an underflow, and 1 when there is not. (i.e. VF set to 1 if VY >= VX).
    uint16_t x = (opcode & 0x0F00) >> 8;
    uint16_t y = (opcode & 0x00F0) >> 4;

    if (V[x] > V[y])
    {
        V[0xF] = 0;
        std::cout << "Borrow" << std::endl;
    }
    else
    {
        V[0xF] = 1;
        std::cout << "No Borrow" << std::endl;
    }

    V[x] = (V[y] - V[x]) & 0xFF;
    std::cout << "V[" << x << "] = V[" << y << "] - V[" << x << "], Applies Borrow if needed" << std::endl;
    pc += 2;
}

void Chip::opcode_8XYE(uint16_t opcode)
{
    //8XYE: Stores the most significant bit of VX in VF and then shifts VX to the left by 1
    //   Least Significant Bit (LSB) : The rightmost bit in a binary number.  2^0
    //   Most Significant Bit  (MSB) : The leftmost bit in a binary number.


    uint16_t x = (opcode & 0x0F00) >> 8;
    V[0xF] = V[x] & 0x80;
    V[x] = V[x] << 1;
    pc += 2;
    std::cout << "Shift V[ " << x << "] << 1 and VF to MSB of VX" << std::endl;
}


void Chip::opcode_9XY0(uint16_t opcode)
{
    // to be implemented
}

void Chip::opcode_ANNN(uint16_t opcode)
{
    // to be implemented
}

void Chip::opcode_BNNN(uint16_t opcode)
{
    // to be implemented
}

void Chip::opcode_CXNN(uint16_t opcode)
{
    // to be implemented
}

void Chip::opcode_DXYN(uint16_t opcode)
{
    // to be implemented
}

void Chip::opcode_EX9E(uint16_t opcode)
{
    // to be implemented
}

void Chip::opcode_EXA1(uint16_t opcode)
{
    // to be implemented
}

void Chip::opcode_FX07(uint16_t opcode)
{
    // to be implemented
}

void Chip::opcode_FX0A(uint16_t opcode)
{
    // to be implemented
}

void Chip::opcode_FX15(uint16_t opcode)
{
    // to be implemented
}

void Chip::opcode_FX18(uint16_t opcode)
{
    // to be implemented
}

void Chip::opcode_FX1E(uint16_t opcode)
{
    // to be implemented
}

void Chip::opcode_FX29(uint16_t opcode)
{
    // to be implemented
}

void Chip::opcode_FX33(uint16_t opcode)
{
    // to be implemented
}

void Chip::opcode_FX55(uint16_t opcode)
{
    // to be implemented
}

void Chip::opcode_FX65(uint16_t opcode)
{
    // to be implemented
}


//========================================================================================================================//
//========================================= Other Functions  =============================================================//
//========================================================================================================================//


// Load the program into memory
void Chip::loadProgram(const std::string& filePath)
{
    std::ifstream input(filePath, std::ios::binary);
    if (!input)
    {
        std::cerr << "Error opening file: " << filePath << std::endl;
        std::exit(EXIT_FAILURE);
    }

    int offset = 0;
    char byte;  // char seems to be standard for reading binary files in cpp
    while (input.read(&byte, 1))
    {
        memory[0x200 + offset] = static_cast<uint8_t>(byte);  //cast to uint8_t as that is the datatype of memory representation of the chip
        offset++;
    }
}

void Chip::loadFontset()
{
    constexpr int fontsetSize = 80; // Size of the fontset array
    for (int i = 0; i < fontsetSize; i++)
    {
        memory[0x50 + i] = ChipFontset::fontset[i] & 0xFF;
    }
}

