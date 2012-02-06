#include <wiring_private.h>
#include <Arduino.h>
#include <avr/pgmspace.h>
#include "DateTime.h"
#include "MatrixDisplay.h"
#include "Font5x8Digits.h"

void RenderTime(DateTime *currentTime) {
  uint8_t NewDisplayArray[16];
  uint8_t i, thisHour;
  uint8_t amPm = 0;

  for(i=0; i<16; i++) {
    NewDisplayArray[i] = 0;
  }

  thisHour = currentTime->hour();

  // Set the minute LEDs
  i = currentTime->minute() % 5;
  switch(i) {
  case 4:
    setPixelFixed(NewDisplayArray, 1, 3);
  case 3:
    setPixelFixed(NewDisplayArray, 1, 2);
  case 2:
    setPixelFixed(NewDisplayArray, 1, 1);
  case 1:
    setPixelFixed(NewDisplayArray, 1, 0);
  case 0:
  default:
    break;
  }

  // MIKE, do your editing from here on

  // Turn on the it, is
  setPixel(NewDisplayArray, 2, 7);
  setPixel(NewDisplayArray, 3, 7);
  setPixel(NewDisplayArray, 5, 7);
  setPixel(NewDisplayArray, 6, 7);

  // Set the minute
  i = currentTime->minute();
  if(i < 5) {
    //o'clock
    setPixel(NewDisplayArray, 2, 0);
    setPixel(NewDisplayArray, 3, 0);
    setPixel(NewDisplayArray, 4, 0);
    setPixel(NewDisplayArray, 5, 0);
    setPixel(NewDisplayArray, 6, 0);
    setPixel(NewDisplayArray, 7, 0);
  } 
  else if(i < 10) {
    //five past
    setPixel(NewDisplayArray, 2, 6);
    setPixel(NewDisplayArray, 3, 6);
    setPixel(NewDisplayArray, 4, 6);
    setPixel(NewDisplayArray, 5, 6);

    setPixel(NewDisplayArray, 6, 5);
    setPixel(NewDisplayArray, 7, 5);
    setPixel(NewDisplayArray, 8, 5);
    setPixel(NewDisplayArray, 9, 5);
    setPixel(NewDisplayArray, 10, 5);
    setPixel(NewDisplayArray, 11, 5);
    setPixel(NewDisplayArray, 12, 5);

    setPixel(NewDisplayArray, 2, 4);
    setPixel(NewDisplayArray, 3, 4);
    setPixel(NewDisplayArray, 4, 4);
    setPixel(NewDisplayArray, 5, 4);
  } 
  else if(i < 15) {
    //ten past
    setPixel(NewDisplayArray, 6, 6);
    setPixel(NewDisplayArray, 7, 6);
    setPixel(NewDisplayArray, 8, 6);

    setPixel(NewDisplayArray, 6, 5);
    setPixel(NewDisplayArray, 7, 5);
    setPixel(NewDisplayArray, 8, 5);
    setPixel(NewDisplayArray, 9, 5);
    setPixel(NewDisplayArray, 10, 5);
    setPixel(NewDisplayArray, 11, 5);
    setPixel(NewDisplayArray, 12, 5);

    setPixel(NewDisplayArray, 2, 4);
    setPixel(NewDisplayArray, 3, 4);
    setPixel(NewDisplayArray, 4, 4);
    setPixel(NewDisplayArray, 5, 4);
  } 
  else if(i < 20) {
    //quarter past
    setPixel(NewDisplayArray, 14, 7);
    setPixel(NewDisplayArray, 9, 6);
    setPixel(NewDisplayArray, 10, 6);
    setPixel(NewDisplayArray, 11, 6);
    setPixel(NewDisplayArray, 12, 6);
    setPixel(NewDisplayArray, 13, 6);
    setPixel(NewDisplayArray, 14, 6);
    setPixel(NewDisplayArray, 15, 6);

    setPixel(NewDisplayArray, 2, 4);
    setPixel(NewDisplayArray, 3, 4);
    setPixel(NewDisplayArray, 4, 4);
    setPixel(NewDisplayArray, 5, 4);
  } 
  else if(i < 25) {
    //twenty past
    setPixel(NewDisplayArray, 8, 7);
    setPixel(NewDisplayArray, 9, 7);
    setPixel(NewDisplayArray, 10, 7);
    setPixel(NewDisplayArray, 11, 7);
    setPixel(NewDisplayArray, 12, 7);
    setPixel(NewDisplayArray, 13, 7);

    setPixel(NewDisplayArray, 6, 5);
    setPixel(NewDisplayArray, 7, 5);
    setPixel(NewDisplayArray, 8, 5);
    setPixel(NewDisplayArray, 9, 5);
    setPixel(NewDisplayArray, 10, 5);
    setPixel(NewDisplayArray, 11, 5);
    setPixel(NewDisplayArray, 12, 5);

    setPixel(NewDisplayArray, 2, 4);
    setPixel(NewDisplayArray, 3, 4);
    setPixel(NewDisplayArray, 4, 4);
    setPixel(NewDisplayArray, 5, 4);
  } 
  else if(i < 30) {
    //twenty five past
    setPixel(NewDisplayArray, 8, 7);
    setPixel(NewDisplayArray, 9, 7);
    setPixel(NewDisplayArray, 10, 7);
    setPixel(NewDisplayArray, 11, 7);
    setPixel(NewDisplayArray, 12, 7);
    setPixel(NewDisplayArray, 13, 7);
    setPixel(NewDisplayArray, 2, 6);
    setPixel(NewDisplayArray, 3, 6);
    setPixel(NewDisplayArray, 4, 6);
    setPixel(NewDisplayArray, 5, 6);

    setPixel(NewDisplayArray, 6, 5);
    setPixel(NewDisplayArray, 7, 5);
    setPixel(NewDisplayArray, 8, 5);
    setPixel(NewDisplayArray, 9, 5);
    setPixel(NewDisplayArray, 10, 5);
    setPixel(NewDisplayArray, 11, 5);
    setPixel(NewDisplayArray, 12, 5);

    setPixel(NewDisplayArray, 2, 4);
    setPixel(NewDisplayArray, 3, 4);
    setPixel(NewDisplayArray, 4, 4);
    setPixel(NewDisplayArray, 5, 4);
  } 
  else if(i < 35) {
    //half past
    setPixel(NewDisplayArray, 2, 5);
    setPixel(NewDisplayArray, 3, 5);
    setPixel(NewDisplayArray, 4, 5);
    setPixel(NewDisplayArray, 5, 5);

    setPixel(NewDisplayArray, 2, 4);
    setPixel(NewDisplayArray, 3, 4);
    setPixel(NewDisplayArray, 4, 4);
    setPixel(NewDisplayArray, 5, 4);
  } 
  else if(i < 40) {
    //twenty five to
    setPixel(NewDisplayArray, 8, 7);
    setPixel(NewDisplayArray, 9, 7);
    setPixel(NewDisplayArray, 10, 7);
    setPixel(NewDisplayArray, 11, 7);
    setPixel(NewDisplayArray, 12, 7);
    setPixel(NewDisplayArray, 13, 7);
    setPixel(NewDisplayArray, 2, 6);
    setPixel(NewDisplayArray, 3, 6);
    setPixel(NewDisplayArray, 4, 6);
    setPixel(NewDisplayArray, 5, 6);

    setPixel(NewDisplayArray, 6, 5);
    setPixel(NewDisplayArray, 7, 5);
    setPixel(NewDisplayArray, 8, 5);
    setPixel(NewDisplayArray, 9, 5);
    setPixel(NewDisplayArray, 10, 5);
    setPixel(NewDisplayArray, 11, 5);
    setPixel(NewDisplayArray, 12, 5);

    setPixel(NewDisplayArray, 14, 5);
    setPixel(NewDisplayArray, 15, 5);
    thisHour++;
  } 
  else if(i < 45) {
    //twenty to
    setPixel(NewDisplayArray, 8, 7);
    setPixel(NewDisplayArray, 9, 7);
    setPixel(NewDisplayArray, 10, 7);
    setPixel(NewDisplayArray, 11, 7);
    setPixel(NewDisplayArray, 12, 7);
    setPixel(NewDisplayArray, 13, 7);

    setPixel(NewDisplayArray, 6, 5);
    setPixel(NewDisplayArray, 7, 5);
    setPixel(NewDisplayArray, 8, 5);
    setPixel(NewDisplayArray, 9, 5);
    setPixel(NewDisplayArray, 10, 5);
    setPixel(NewDisplayArray, 11, 5);
    setPixel(NewDisplayArray, 12, 5);

    setPixel(NewDisplayArray, 14, 5);
    setPixel(NewDisplayArray, 15, 5);
    thisHour++;
  } 
  else if(i < 50) {
    //quarter to
    setPixel(NewDisplayArray, 14, 7);
    setPixel(NewDisplayArray, 9, 6);
    setPixel(NewDisplayArray, 10, 6);
    setPixel(NewDisplayArray, 11, 6);
    setPixel(NewDisplayArray, 12, 6);
    setPixel(NewDisplayArray, 13, 6);
    setPixel(NewDisplayArray, 14, 6);
    setPixel(NewDisplayArray, 15, 6);

    setPixel(NewDisplayArray, 14, 5);
    setPixel(NewDisplayArray, 15, 5);
    thisHour++;
  } 
  else if(i < 55) {
    //ten to
    setPixel(NewDisplayArray, 6, 6);
    setPixel(NewDisplayArray, 7, 6);
    setPixel(NewDisplayArray, 8, 6);

    setPixel(NewDisplayArray, 6, 5);
    setPixel(NewDisplayArray, 7, 5);
    setPixel(NewDisplayArray, 8, 5);
    setPixel(NewDisplayArray, 9, 5);
    setPixel(NewDisplayArray, 10, 5);
    setPixel(NewDisplayArray, 11, 5);
    setPixel(NewDisplayArray, 12, 5);

    setPixel(NewDisplayArray, 14, 5);
    setPixel(NewDisplayArray, 15, 5);
    thisHour++;
  } 
  else {
    //five to
    setPixel(NewDisplayArray, 2, 6);
    setPixel(NewDisplayArray, 3, 6);
    setPixel(NewDisplayArray, 4, 6);
    setPixel(NewDisplayArray, 5, 6);

    setPixel(NewDisplayArray, 6, 5);
    setPixel(NewDisplayArray, 7, 5);
    setPixel(NewDisplayArray, 8, 5);
    setPixel(NewDisplayArray, 9, 5);
    setPixel(NewDisplayArray, 10, 5);
    setPixel(NewDisplayArray, 11, 5);
    setPixel(NewDisplayArray, 12, 5);

    setPixel(NewDisplayArray, 14, 5);
    setPixel(NewDisplayArray, 15, 5);
    thisHour++;
  }

  if(thisHour > 24) {
    amPm = 0;
    thisHour = 1;
  } 
  else if(thisHour == 24) {
    thisHour = 12;
    amPm = 0;
  } 
  else if(thisHour > 12) {
    thisHour -= 12;
    amPm = 1;
  } 
  else if(thisHour == 12) {
    thisHour = 12;
    amPm = 1;
  } 
  else {
    amPm = 0;
  }

  // Set the hour
  i = thisHour;
  if(i > 12) i -= 12;
  if(i == 0) i = 12;
  switch(i) {
  case 12:
    setPixel(NewDisplayArray, 7, 4);
    setPixel(NewDisplayArray, 8, 4);
    setPixel(NewDisplayArray, 9, 4);
    setPixel(NewDisplayArray, 10, 4);
    setPixel(NewDisplayArray, 11, 4);
    setPixel(NewDisplayArray, 12, 4);
    break;
  case 11:
    setPixel(NewDisplayArray, 10, 3);
    setPixel(NewDisplayArray, 11, 3);
    setPixel(NewDisplayArray, 12, 3);
    setPixel(NewDisplayArray, 13, 3);
    setPixel(NewDisplayArray, 14, 3);
    setPixel(NewDisplayArray, 15, 3);
    break;
  case 10:
    setPixel(NewDisplayArray, 9, 2);
    setPixel(NewDisplayArray, 10, 2);
    setPixel(NewDisplayArray, 11, 2);
    break;
  case 9:
    setPixel(NewDisplayArray, 12, 1);
    setPixel(NewDisplayArray, 13, 1);
    setPixel(NewDisplayArray, 14, 1);
    setPixel(NewDisplayArray, 15, 1);
    break;
  case 8:
    setPixel(NewDisplayArray, 7, 1);
    setPixel(NewDisplayArray, 8, 1);
    setPixel(NewDisplayArray, 9, 1);
    setPixel(NewDisplayArray, 10, 1);
    setPixel(NewDisplayArray, 11, 1);
    break;
  case 7:
    setPixel(NewDisplayArray, 2, 1);
    setPixel(NewDisplayArray, 3, 1);
    setPixel(NewDisplayArray, 4, 1);
    setPixel(NewDisplayArray, 5, 1);
    setPixel(NewDisplayArray, 6, 1);
    break;
  case 6:
    setPixel(NewDisplayArray, 2, 3);
    setPixel(NewDisplayArray, 3, 3);
    setPixel(NewDisplayArray, 4, 3);
    break;
  case 5:
    setPixel(NewDisplayArray, 5, 2);
    setPixel(NewDisplayArray, 6, 2);
    setPixel(NewDisplayArray, 7, 2);
    setPixel(NewDisplayArray, 8, 2);
    break;
  case 4:
    setPixel(NewDisplayArray, 12, 2);
    setPixel(NewDisplayArray, 13, 2);
    setPixel(NewDisplayArray, 14, 2);
    setPixel(NewDisplayArray, 15, 2);
    break;
  case 3:
    setPixel(NewDisplayArray, 5, 3);
    setPixel(NewDisplayArray, 6, 3);
    setPixel(NewDisplayArray, 7, 3);
    setPixel(NewDisplayArray, 8, 3);
    setPixel(NewDisplayArray, 9, 3);
    break;
  case 2:
    setPixel(NewDisplayArray, 2, 2);
    setPixel(NewDisplayArray, 3, 2);
    setPixel(NewDisplayArray, 4, 2);
    break;
  case 1:
    setPixel(NewDisplayArray, 13, 4);
    setPixel(NewDisplayArray, 14, 4);
    setPixel(NewDisplayArray, 15, 4);
    break;
  default:
    break;
  }

  // Set AM / PM
  if(!amPm) {
    setPixel(NewDisplayArray, 9, 0);
    setPixel(NewDisplayArray, 10, 0);
  } 
  else {
    setPixel(NewDisplayArray, 11, 0);
    setPixel(NewDisplayArray, 12, 0);
  }

  // Copy the new display buffer to the main buffer when it's done drawing
  while(!DisplayArrayReady);
  memcpy((uint8_t *)DisplayArray, NewDisplayArray, 16);
}

// MIKE, don't edit this part

void RenderTimeSeconds(DateTime *currentTime) {
  uint8_t NewDisplayArray[16];
  uint8_t i, j=0;

  for(i=0; i<16; i++) {
    NewDisplayArray[i] = 0;
  }

  i = currentTime->second();
  i /= 10;
  if(i != 0) {
    j = 13;
    NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][0]]);
    NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][1]]);
    NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][2]]);
    NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][3]]);
    NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][4]]);
    j--;
  } 
  else {
    j = 10;
  }

  i = currentTime->second();
  i %= 10;
  NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][0]]);
  NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][1]]);
  NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][2]]);
  NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][3]]);
  NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][4]]);

  // Copy the new display buffer to the main buffer when it's done drawing
  while(!DisplayArrayReady);
  memcpy((uint8_t *)DisplayArray, NewDisplayArray, 16);    
}

void RenderDigits(uint8_t dispNumber, uint8_t setQuadrant) {
  uint8_t NewDisplayArray[16];
  uint8_t i, j=0;

  for(i=0; i<16; i++) {
    NewDisplayArray[i] = 0;
  }

  if(setQuadrant == 1) {
    NewDisplayArray[15] = 0b00000011;
  } 
  else if(setQuadrant == 2) {
    NewDisplayArray[15] = 0b00011000;
  } 
  else if(setQuadrant == 3) {
    NewDisplayArray[15] = 0b11000000;
  } 
  else if(setQuadrant == 4) {
    NewDisplayArray[2] = 0b11000000;
  } 
  else if(setQuadrant == 5) {
    NewDisplayArray[2] = 0b00011000;
  } 
  else if(setQuadrant == 6) {
    NewDisplayArray[2] = 0b00000011;
  }

  if(dispNumber > 99) {
    dispNumber = 99;
  }

  i = dispNumber;
  i /= 10;
  if(i != 0) {
    j = 13;
    NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][0]]);
    NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][1]]);
    NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][2]]);
    NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][3]]);
    NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][4]]);
    j--;
  } 
  else {
    j = 10;
  }

  i = dispNumber;
  i %= 10;
  NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][0]]);
  NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][1]]);
  NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][2]]);
  NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][3]]);
  NewDisplayArray[j--] = pgm_read_byte(&BitReverseTable256[FiveByEightDigitFont[i][4]]);

  // Copy the new display buffer to the main buffer when it's done drawing
  while(!DisplayArrayReady);
  memcpy((uint8_t *)DisplayArray, NewDisplayArray, 16);
}

