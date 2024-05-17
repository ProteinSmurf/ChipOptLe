#pragma once
#include <iostream>

// A CHIP - 8 emulator should have a built-in font, with sprite data representing the hexadecimal numbers from 0 through F
// Should be stored in memory anywhere between 0x000 and 0x1FF ( before 0x200 where ROM starts) , but convetionally they are stored starting from 0x50  ( reason ??? unknown ) 

class ChipFontset 
{
    public:
        /**
         * Fontset in bytes
         * Memory position 0x50
         */
        static const int fontset[];
    };

// Fontset is used to render the hexadecimal digits (0-9, A-F) on the chip display. 
const int ChipFontset::fontset[] = 
{
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

/*
    Showcase:

     "0"        Binary        Hexadecimal              |     "1"       Binary       Hexadecimal
     ****       11110000      0xF0                     |      *         00100000     0x20
     *  *       10010000      0x90                     |     **         01100000     0x60
     *  *       10010000      0x90                     |      *         00100000     0x20
     *  *       10010000      0x90                     |      *         00100000     0x20
     ****       11110000      0xF0                     |     ***        01110000     0x70

     "2"        Binary        Hexadecimal              |        "3"       Binary     Hexadecimal
     ****       11110000      0xF0                     |      ****      11110000     0xF0
        *       00010000      0x10                     |         *      00010000     0x10
     ****       11110000      0xF0                     |      ****      11110000     0xF0
     *          10000000      0x80                     |         *      00010000     0x10
     ****       11110000      0xF0                     |      ****      11110000     0xF0

     "4"        Binary        Hexadecimal              |        "5"       Binary     Hexadecimal
     *  *       10010000      0x90                     |      ****      11110000     0xF0
     *  *       10010000      0x90                     |      *         10000000     0x80
     ****       11110000      0xF0                     |      ****      11110000     0xF0
        *       00010000      0x10                     |         *      00010000     0x10
        *       00010000      0x10                     |      ****      11110000     0xF0

     "6"        Binary        Hexadecimal              |        "7"       Binary     Hexadecimal
     ****       11110000      0xF0                     |      ****      11110000     0xF0
     *          10000000      0x80                     |         *      00010000     0x10
     ****       11110000      0xF0                     |        *       00100000     0x20
     *  *       10010000      0x90                     |       *        01000000     0x40
     ****       11110000      0xF0                     |       *        01000000     0x40

     "8"        Binary        Hexadecimal              |        "9"       Binary     Hexadecimal
     ****       11110000      0xF0                     |      ****      11110000     0xF0
     *  *       10010000      0x90                     |      *  *      10010000     0x90
     ****       11110000      0xF0                     |      ****      11110000     0xF0
     *  *       10010000      0x90                     |         *      00010000     0x10
     ****       11110000      0xF0                     |      ****      11110000     0xF0

     "A"        Binary        Hexadecimal              |        "B"       Binary     Hexadecimal
     ****       11110000      0xF0                     |      ***       11100000     0xE0
     *  *       10010000      0x90                     |      *  *      10010000     0x90
     ****       11110000      0xF0                     |      ***       11100000     0xE0
     *  *       10010000      0x90                     |      *  *      10010000     0x90
     *  *       10010000      0x90                     |      ***       11100000     0xE0

     "C"        Binary        Hexadecimal              |        "D"       Binary     Hexadecimal
     ****       11110000      0xF0                     |      ***       11100000     0xE0
     *          10000000      0x80                     |      *  *      10010000     0x90
     *          10000000      0x80                     |      *  *      10010000     0x90
     *          10000000      0x80                     |      *  *      10010000     0x90
     ****       11110000      0xF0                     |      ***       11100000     0xE0

     "E"        Binary        Hexadecimal              |        "F"       Binary     Hexadecimal
     ****       11110000      0xF0                     |      ****      11110000     0xF0
     *          10000000      0x80                     |      *         10000000     0x80
     ****       11110000      0xF0                     |      ****      11110000     0xF0
     *          10000000      0x80                     |      *         10000000     0x80
     ****       11110000      0xF0                     |      *         10000000     0x80


*/