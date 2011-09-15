volatile uint8_t displayArray[16];
volatile uint8_t currentCol = 0;
uint8_t columnMaskH[] = {0b00001000, 0b00010000, 0b00000100, 0b00100000, 0b00000010, 0b01000000, 0b00000001, 0b10000000, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t columnMaskL[] = {0, 0, 0, 0, 0, 0, 0, 0, 0b00001000, 0b00010000, 0b00000100, 0b00100000, 0b00000010, 0b01000000, 0b00000001, 0b10000000};

#define DataPinPort PORTD
#define DataPinTris DDRD
#define DataPinBit 3

#define LoadPinPort PORTD
#define LoadPinTris DDRD
#define LoadPinBit 4

#define ClockPinPort PORTB
#define ClockPinTris DDRB
#define ClockPinBit 6

#define BrightPin 10

#include <wiring_private.h>
#include "TimerOne.h"

void setup() {
  pinMode(DataPinBit, OUTPUT);
  pinMode(LoadPinBit, OUTPUT);
  pinMode(BrightPin, OUTPUT);
  sbi(ClockPinTris, ClockPinBit);
  digitalWrite(BrightPin, 0);
  Timer1.initialize(625);
  Timer1.attachInterrupt(displayTimerHandler);
  Serial.begin(19200);
  Serial.println("Starting");
  uint8_t i, j;
  for(i=0;i<16;i++) {
    for(j=0;j<8;j++) {
      clearPixel(i, j);
    }
  }
//  setPixel(3,3);
}

void loop() {
  uint8_t i, j;
  for(i=0;i<16;i++) {
    for(j=0;j<8;j++) {
      setPixel(i, j);
      delay(250);
    }
  }
  
  for(i=0;i<16;i++) {
    for(j=0;j<8;j++) {
      clearPixel(i, j);
      delay(250);
    }
  }
}

void setPixel(uint8_t xCoord, uint8_t yCoord) {
  xCoord %= 16;
  yCoord %= 8;
  displayArray[xCoord] |= _BV(yCoord);
}

void clearPixel(uint8_t xCoord, uint8_t yCoord) {
  xCoord %= 16;
  yCoord %= 8;
  displayArray[xCoord] &= ~(_BV(yCoord));
}

void displayTimerHandler() {
  cbi(LoadPinPort, LoadPinBit);

  digitalWrite(BrightPin, 1);

  srWrite(0);
  srWrite(displayArray[currentCol]);
  srWrite(columnMaskH[currentCol]);
  srWrite(columnMaskL[currentCol]);

  sbi(LoadPinPort, LoadPinBit);
  
  digitalWrite(BrightPin, 0);
  
  currentCol++;
  currentCol %= 16;
}

void srWrite(uint8_t byteToWrite) {
  uint8_t i;
  
  for(i=0; i<8; i++) {
    if(byteToWrite & (1 << i)) {
      sbi(DataPinPort, DataPinBit);
    } else {
      cbi(DataPinPort, DataPinBit);
    }
    sbi(ClockPinPort, ClockPinBit);
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    cbi(ClockPinPort, ClockPinBit);
    __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  }
  cbi(ClockPinPort, ClockPinBit);
}
