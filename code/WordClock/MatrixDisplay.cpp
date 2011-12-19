#include "MatrixDisplay.h"

volatile uint8_t DisplayArray[16];
volatile uint8_t DisplayArrayReady = 1;

uint8_t columnMaskH[] = {0b00001000, 0b00010000, 0b00000100, 0b00100000, 0b00000010, 0b01000000, 0b00000001, 0b10000000, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t columnMaskL[] = {0, 0, 0, 0, 0, 0, 0, 0, 0b00001000, 0b00010000, 0b00000100, 0b00100000, 0b00000010, 0b01000000, 0b00000001, 0b10000000};

void ClearDisplayArray() {
    uint8_t i;
    
    for(i = 0; i < 16; i++) {
        DisplayArray[i] = 0;
    }
}

void FastShiftOut(uint8_t shiftVal) {
    uint8_t i;
    
    cbi(ClockPinPort, ClockPinBit);
    
    for(i=0; i<8; i++) {
        if(shiftVal & (1 << i)) {
            sbi(DataPinPort, DataPinBit);
        } else {
            cbi(DataPinPort, DataPinBit);
        }
        sbi(ClockPinPort, ClockPinBit);
        asm("nop\n nop\n nop\n");
        cbi(ClockPinPort, ClockPinBit);
        asm("nop\n nop\n nop\n");
    }
}

void OutputDisplayColumn(uint8_t colNum) {
    DisplayArrayReady = 0;
    cbi(LoadPinPort, LoadPinBit);

    FastShiftOut(0);
    FastShiftOut(DisplayArray[colNum]);
    FastShiftOut(columnMaskH[colNum]);
    FastShiftOut(columnMaskL[colNum]);

    sbi(LoadPinPort, LoadPinBit);
  
    if(colNum == 15) {
        DisplayArrayReady = 1;
    }
}

void SetupFastShiftOut() {
    sbi(ClockPinTris, ClockPinBit);
    sbi(DataPinTris, DataPinBit);
    sbi(LoadPinTris, LoadPinBit);
    cbi(ClockPinPort, ClockPinBit);
    cbi(DataPinPort, DataPinBit);
    cbi(LoadPinPort, LoadPinBit);
}
