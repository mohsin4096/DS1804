#include "DS1804.h"

// example code for PIC 18F2550
// configure pins in DS1804.h

void init() {
    // set AN0 - AN12 as digital pins
    ADCON1 = 0xFF;

    // set ports direction
    TRISA = 0x00;
}

void main() {
    init();

    initialize();



    while(1){
        setWiperPosition(50);
        Delay_ms(1000);
        setWiperPosition(0);
        Delay_ms(1000);
    }
}