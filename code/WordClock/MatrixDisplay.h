#include <Arduino.h> 
#include <wiring_private.h>

#ifndef SURREALITYLABS_MATRIXDISPLAY
#define SURREALITYLABS_MATRIXDISPLAY

#define setPixel(x, y, z) (x[17-y] |= 1 << (7-z))
//#define setPixel(x, y, z) (x[y] |= (1 << z))

#define setPixelFixed(x, y, z) (x[y] |= (1 << z))

#define DataPinPort PORTD
#define DataPinTris DDRD
#define DataPinBit 3

#define LoadPinPort PORTD
#define LoadPinTris DDRD
#define LoadPinBit 4

#define ClockPinPort PORTB
#define ClockPinTris DDRB
#define ClockPinBit 6

extern volatile uint8_t DisplayArray[16];
extern volatile uint8_t DisplayArrayReady;

extern uint8_t columnMaskH[];
extern uint8_t columnMaskL[];

void OutputDisplayColumn(uint8_t colNum);
void ClearDisplayArray();
void FastShiftOut(uint8_t shiftVal);
void SetupFastShiftOut();

#endif
