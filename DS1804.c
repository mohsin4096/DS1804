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
#include "DS1804.h"

uint8_t m_steps;
uint32_t m_maxResistance;
uint8_t m_wiperPosition;

void initialize(){
    m_INCpin = 1;

    m_maxResistance = M_MAXRESISTANCE;
    m_steps = M_STEPS;
    m_wiperPosition = 0;
}

// Getter Functions
// --------------------------------------------------------------------------------
uint8_t isLocked(){
    return m_CSpin;
}

uint8_t getWiperPosition(){
    return m_wiperPosition;
}

uint32_t getResistance(){
    return wiperPositionToResistance(getWiperPosition());
}

// Helpful Mapper Functions
// --------------------------------------------------------------------------------
uint8_t resistanceToWiperPosition(uint32_t wantedResistance){
    uint32_t newResistance = constrain(wantedResistance, 0, m_maxResistance);
    return map(newResistance, 0, m_maxResistance, 0, m_steps);
}

uint32_t wiperPositionToResistance(uint8_t proposedWiperPosition){
    uint8_t newWiperPosition = constrain(proposedWiperPosition, 0, m_steps);
    return map(newWiperPosition, 0, m_steps, 0, m_maxResistance);
}

uint32_t resistanceToActualResistance(uint32_t wantedResistance){
    return wiperPositionToResistance(resistanceToWiperPosition(wantedResistance));
}

// Setter Functions
// --------------------------------------------------------------------------------
void lock(){
    m_CSpin = 1;
    Delay_us(1);
}

void unlock(){
    m_CSpin = 0;
    Delay_us(CS_TO_INC_SETUP);
}

void write(){
    m_CSpin = 0;
    m_INCpin = 1;
    m_CSpin = 1;
    Delay_us(WIPER_STORAGE_TIME);
    m_CSpin = 0;
    Delay_us(WIPER_STORAGE_TIME - CS_DESELECT_TIME);
}

void setToZero(){
    m_UDpin = 0;
    Delay_us(1);
    transmitPulses(100);
}

uint8_t setWiperPosition(uint8_t wiperPosition){
    if (!isLocked()) {
        // work out the number of pulses to get to new position
        uint8_t newWiperPosition = constrain(wiperPosition, 0, m_steps);
        uint8_t pulses = abs(newWiperPosition - m_wiperPosition);

        // set the UD pin accordingly
        m_UDpin = newWiperPosition > m_wiperPosition;
        Delay_us(1);

        // send the INC pulses
        transmitPulses(pulses);

        m_wiperPosition = newWiperPosition;
    }
    return m_wiperPosition;
}

uint8_t overrideWiperPosition(uint8_t wiperPosition){
    return m_wiperPosition = constrain(wiperPosition, 0, m_steps);
}

uint32_t setResistance(uint32_t wantedResistance){
    if (!isLocked()) {
        setWiperPosition(resistanceToWiperPosition(wantedResistance));
    }
    return getResistance();
}

void transmitPulses(uint8_t wantedPulses){
    uint8_t n = 0;
    constrain(wantedPulses, 0, m_steps);
    m_INCpin = 1;
    Delay_us(1);
    for (n = 0; n < wantedPulses; n++){
        m_INCpin = 0;
        Delay_us(1);
        m_INCpin = 1;
        Delay_us(1);
    }
}



// Helper Functions
// --------------------------------------------------------------------------------
uint32_t constrain(uint32_t x, uint32_t a, uint32_t b){
    if (x < a)
        return a;
    if (x > b)
        return b;
    return x;
}

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint32_t abs(int32_t x){
    if (x >= 0)
        return x;
    else
        return -x;
}
