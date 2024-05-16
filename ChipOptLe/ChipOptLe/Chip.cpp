#include "Chip.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>


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

    //Decode opcode
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
        default: //0NNN: Calls RCA 1802 Program at address NNN
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
        case 0x0002: //8XY0: Sets VX to the value of VY
        {
            opcode_8XY2(opcode);
            break;
        }
        case 0x0003: //8XY2: Sets VX to VX AND VY
        {
            opcode_8XY3(opcode);
            break;
        }
        case 0x0004: //Adds VY to VX. VF is set to 1 when carry applies else to 0
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
    // to be implemented
}

void Chip::opcode_00EE()
{
    // to be implemented
}

void Chip::opcode_1NNN(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_2NNN(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_3XNN(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_4XNN(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_5XY0(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_6XNN(unsigned short opcode)
{
    // to be implemented
}


void Chip::opcode_7XNN(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_8XY0(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_8XY1(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_8XY2(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_8XY3(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_8XY4(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_8XY5(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_8XY6(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_8XY7(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_8XYE(unsigned short opcode)
{
    // to be implemented
}


void Chip::opcode_9XY0(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_ANNN(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_BNNN(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_CXNN(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_DXYN(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_EX9E(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_EXA1(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_FX07(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_FX0A(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_FX15(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_FX18(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_FX1E(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_FX29(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_FX33(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_FX55(unsigned short opcode)
{
    // to be implemented
}

void Chip::opcode_FX65(unsigned short opcode)
{
    // to be implemented
}


//========================================================================================================================//
//========================================= Other Functions  =============================================================//
//========================================================================================================================//


//loadprogram - to be implemented
