/*
    DS1804.cpp - a library for controlling a DS1804 Nonvolatile Trimmer Potentiometer
    http://datasheets.maximintegrated.com/en/ds/DS1804.pdf

    The MIT License (MIT)

    Copyright (c) 2016 tom-dudman

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
#ifndef DS1804_H
#define DS1804_H

#include <stdint.h>

#define m_CSpin PORTA.RA0
#define m_INCpin PORTA.RA1
#define m_UDpin PORTA.RA2

#define M_MAXRESISTANCE DS1804_HUNDRED
#define M_STEPS 99

#define CS_TO_INC_SETUP 1
#define CS_DESELECT_TIME 1
#define WIPER_STORAGE_TIME 10000

#define DS1804_TEN 10000
#define DS1804Z_TEN 10000
#define DS1804U_TEN 10000
#define DS1804_FIFTY 50000
#define DS1804Z_FIFTY 50000
#define DS1804U_FIFTY 50000
#define DS1804_HUNDRED 100000
#define DS1804Z_HUNDRED 100000
#define DS1804U_HUNDRED 100000

extern uint8_t m_steps;
extern uint32_t m_maxResistance;
extern uint8_t m_wiperPosition;

void initialize();

// Getter Functions
// --------------------------------------------------------------------------------
uint8_t isLocked();
uint32_t getResistance();
uint8_t getWiperPosition();

// Helpful Mapper Functions
// --------------------------------------------------------------------------------
uint8_t resistanceToWiperPosition(uint32_t wantedResistance);
uint32_t wiperPositionToResistance(uint8_t proposedWiperPosition);
uint32_t resistanceToActualResistance(uint32_t wantedResistance);

// Setter Functions
// --------------------------------------------------------------------------------
void lock();
void unlock();
void write();
void setToZero();
uint8_t setWiperPosition(uint8_t wiperPosition);
uint8_t overrideWiperPosition(uint8_t wiperPosition);
uint32_t setResistance(uint32_t wantedResistance);

// Internal Functions
// --------------------------------------------------------------------------------
void transmitPulses(uint8_t wantedPulses);

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);
uint32_t constrain(uint32_t x, uint32_t a, uint32_t b);
uint32_t abs(uint32_t x);

#endif    // ifndef DS1804_H