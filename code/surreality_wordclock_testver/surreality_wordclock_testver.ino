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
#include <Wire.h>
#include "RTClib.h"

RTC_MCP794XX RTC;

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
  
      Wire.begin();
    RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
//  setPixel(3,3);
}

void loop() {
  DateTime now = RTC.now();

  uint8_t i, j;
  for(i=0;i<16;i++) {
    for(j=0;j<8;j++) {
      clearPixel(i, j);
    }
  }
  
  changeIt(1);
  changeIs(1);
  changeOclock(1);
  
  if(now.minute() < 5) {
    changeTwenty(0);
    changeHalf(0);
    changeTen(0);
    changeQuarter(0);
    changeFive(0);
    changeTo(0);
    changePast(0);        
  } else if(now.minute() < 10) {
    changeTwenty(0);
    changeHalf(0);
    changeTen(0);
    changeQuarter(0);
    changeFive(1);
    changeTo(0);
    changePast(1);    
  } else if(now.minute() < 15) {
    changeTwenty(0);
    changeHalf(0);
    changeTen(1);
    changeQuarter(0);
    changeFive(0);
    changeTo(0);
    changePast(1);
  } else if(now.minute() < 20) {
    changeTwenty(0);
    changeHalf(0);
    changeTen(0);
    changeQuarter(1);
    changeFive(0);
    changeTo(0);
    changePast(1);
  } else if(now.minute() < 25) {
    changeTwenty(1);
    changeHalf(0);
    changeTen(0);
    changeQuarter(0);
    changeFive(0);
    changeTo(0);
    changePast(1);
  } else if(now.minute() < 30) {
    changeTwenty(1);
    changeHalf(0);
    changeTen(0);
    changeQuarter(0);
    changeFive(1);
    changeTo(0);
    changePast(1);    
  } else if(now.minute() < 35) {
    changeTwenty(0);
    changeHalf(1);
    changeTen(0);
    changeQuarter(0);
    changeFive(0);
    changeTo(0);
    changePast(1);    
  } else if(now.minute() < 40) {
    changeTwenty(1);
    changeHalf(0);
    changeTen(0);
    changeQuarter(0);
    changeFive(1);
    changeTo(1);
    changePast(0);
  } else if(now.minute() < 45) {
    changeTwenty(1);
    changeHalf(0);
    changeTen(0);
    changeQuarter(0);
    changeFive(0);
    changeTo(1);
    changePast(0);
  } else if(now.minute() < 50) {
    changeTwenty(0);
    changeHalf(0);
    changeTen(0);
    changeQuarter(1);
    changeFive(0);
    changeTo(1);
    changePast(0);
  } else if(now.minute() < 55) {
    changeTwenty(0);
    changeHalf(0);
    changeTen(1);
    changeQuarter(0);
    changeFive(0);
    changeTo(1);
    changePast(0);
  } else  {
    changeTwenty(0);
    changeHalf(0);
    changeTen(0);
    changeQuarter(0);
    changeFive(1);
    changeTo(1);
    changePast(0);
  }
  
  uint8_t tempHour;
  tempHour = now.hour();
  if(now.minute() > 35) {
    tempHour++;
  }
  if(tempHour == 24) tempHour = 0;
  if(tempHour > 11) {
    changeAM(0);
    changePM(1);
    tempHour = tempHour - 12;
  } else {
    changeAM(1);
    changePM(0);
  }
  
  if(tempHour == 0) {
    changeTwelveHour(1);
  } else {
    changeTwelveHour(0);
  }
  
  if(tempHour == 1) {
    changeOneHour(1);
  } else {
    changeOneHour(0);
  }
  
  if(tempHour == 2) {
    changeTwoHour(1);
  } else {
    changeTwoHour(0);
  }
  
  if(tempHour == 3) {
    changeThreeHour(1);
  } else {
    changeThreeHour(0);
  }

  if(tempHour == 4) {
    changeFourHour(1);
  } else {
    changeFourHour(0);
  }
  
  if(tempHour == 5) {
    changeFiveHour(1);
  } else {
    changeFiveHour(0);
  }
  
  if(tempHour == 6) {
    changeSixHour(1);
  } else {
    changeSixHour(0);
  }
  
  if(tempHour == 7) {
    changeSevenHour(1);
  } else {
    changeSevenHour(0);
  }
  
  if(tempHour == 8) {
    changeEightHour(1);
  } else {
    changeEightHour(0);
  }
  
  if(tempHour == 9) {
    changeNineHour(1);
  } else {
    changeNineHour(0);
  }
  
  if(tempHour == 10) {
    changeTenHour(1);
  } else {
    changeTenHour(0);
  }
  
  if(tempHour == 11) {
    changeElevenHour(1);
  } else {
    changeElevenHour(0);
  }
  
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
  delay(1000);
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

void changeIt(uint8_t state) {
    if(state) {
        setPixel(2, 7);
        setPixel(3, 7);
    } else {
        clearPixel(2, 7);
        clearPixel(2, 7);
    }
}

void changeIs(uint8_t state) {
    if(state) {
        setPixel(5, 7);
        setPixel(6, 7);
    } else {
        clearPixel(5, 7);
        clearPixel(6, 7);
    }
}

void changeTwenty(uint8_t state) {
    if(state) {
        setPixel(8, 7);
        setPixel(9, 7);
        setPixel(10, 7);
        setPixel(11, 7);
        setPixel(12, 7);
        setPixel(13, 7);
    } else {
        clearPixel(8, 7);
        clearPixel(9, 7);
        clearPixel(10, 7);
        clearPixel(11, 7);
        clearPixel(12, 7);
        clearPixel(13, 7);
    }
}

void changeHalf(uint8_t state) {
    if(state) {
        setPixel(2, 6);
        setPixel(3, 6);
        setPixel(4, 6);
        setPixel(5, 6);
    } else {
        clearPixel(2, 6);
        clearPixel(3, 6);
        clearPixel(4, 6);
        clearPixel(5, 6);
    }
}

void changeTen(uint8_t state) {
    if(state) {
        setPixel(6, 6);
        setPixel(7, 6);
        setPixel(8, 6);
    } else {
        clearPixel(6, 6);
        clearPixel(7, 6);
        clearPixel(8, 6);
    }
}

void changeQuarter(uint8_t state) {
    if(state) {
        setPixel(9, 6);
        setPixel(10, 6);
        setPixel(11, 6);
        setPixel(12, 6);
        setPixel(13, 6);
        setPixel(14, 6);
        setPixel(15, 6);
    } else {
        clearPixel(9, 6);
        clearPixel(10, 6);
        clearPixel(11, 6);
        clearPixel(12, 6);
        clearPixel(13, 6);
        clearPixel(14, 6);
        clearPixel(15, 6);
    }
}

void changeFive(uint8_t state) {
    if(state) {
        setPixel(2, 5);
        setPixel(3, 5);
        setPixel(4, 5);
        setPixel(5, 5);
    } else {
        clearPixel(2, 5);
        clearPixel(3, 5);
        clearPixel(4, 5);
        clearPixel(5, 5);
    }
}

void changeTo(uint8_t state) {
    if(state) {
        setPixel(7, 5);
        setPixel(8, 5);
    } else {
        clearPixel(7, 5);
        clearPixel(8, 5);
    }
}

void changePast(uint8_t state) {
    if(state) {
        setPixel(10, 5);
        setPixel(11, 5);
        setPixel(12, 5);
        setPixel(13, 5);
    } else {
        clearPixel(10, 5);
        clearPixel(11, 5);
        clearPixel(12, 5);
        clearPixel(13, 5);
    }
}

void changeTwelveHour(uint8_t state) {
    if(state) {
        setPixel(2, 4);
        setPixel(3, 4);
        setPixel(4, 4);
        setPixel(5, 4);
        setPixel(6, 4);
        setPixel(7, 4);
    } else {
        clearPixel(2, 4);
        clearPixel(3, 4);
        clearPixel(4, 4);
        clearPixel(5, 4);
        clearPixel(6, 4);
        clearPixel(7, 4);
    }
}

void changeElevenHour(uint8_t state) {
    if(state) {
        setPixel(9, 4);
        setPixel(10, 4);
        setPixel(11, 4);
        setPixel(12, 4);
        setPixel(13, 4);
        setPixel(14, 4);
    } else {
        clearPixel(9, 4);
        clearPixel(10, 4);
        clearPixel(11, 4);
        clearPixel(12, 4);
        clearPixel(13, 4);
        clearPixel(14, 4);
    }
}

void changeOneHour(uint8_t state) {
    if(state) {
        setPixel(2, 3);
        setPixel(3, 3);
        setPixel(4, 3);
    } else {
        clearPixel(2, 3);
        clearPixel(3, 3);
        clearPixel(4, 3);
    }
}

void changeSixHour(uint8_t state) {
    if(state) {
        setPixel(5, 3);
        setPixel(6, 3);
        setPixel(7, 3);
    } else {
        clearPixel(5, 3);
        clearPixel(6, 3);
        clearPixel(7, 3);
    }
}

void changeTwoHour(uint8_t state) {
    if(state) {
        setPixel(8, 3);
        setPixel(9, 3);
        setPixel(10, 3);
    } else {
        clearPixel(8, 3);
        clearPixel(9, 3);
        clearPixel(10, 3);
    }
}

void changeFiveHour(uint8_t state) {
    if(state) {
        setPixel(12, 3);
        setPixel(13, 3);
        setPixel(14, 3);
        setPixel(15, 3);
    } else {
        clearPixel(12, 3);
        clearPixel(13, 3);
        clearPixel(14, 3);
        clearPixel(15, 3);
    }
}

void changeThreeHour(uint8_t state) {
    if(state) {
        setPixel(2, 2);
        setPixel(3, 2);
        setPixel(4, 2);
        setPixel(5, 2);
        setPixel(6, 2);
    } else {
        clearPixel(2, 2);
        clearPixel(3, 2);
        clearPixel(4, 2);
        clearPixel(5, 2);
        clearPixel(6, 2);
    }
}

void changeTenHour(uint8_t state) {
    if(state) {
        setPixel(8, 2);
        setPixel(9, 2);
        setPixel(10, 2);
    } else {
        clearPixel(8, 2);
        clearPixel(9, 2);
        clearPixel(10, 2);
    }
}

void changeFourHour(uint8_t state) {
    if(state) {
        setPixel(12, 2);
        setPixel(13, 2);
        setPixel(14, 2);
        setPixel(15, 2);
    } else {
        clearPixel(12, 2);
        clearPixel(13, 2);
        clearPixel(14, 2);
        clearPixel(15, 2);
    }
}

void changeSevenHour(uint8_t state) {
    if(state) {
        setPixel(2, 1);
        setPixel(3, 1);
        setPixel(4, 1);
        setPixel(5, 1);
        setPixel(6, 1);
    } else {
        clearPixel(2, 1);
        clearPixel(3, 1);
        clearPixel(4, 1);
        clearPixel(5, 1);
        clearPixel(6, 1);
    }
}

void changeEightHour(uint8_t state) {
    if(state) {
        setPixel(7, 1);
        setPixel(8, 1);
        setPixel(9, 1);
        setPixel(10, 1);
        setPixel(11, 1);
    } else {
        clearPixel(7, 1);
        clearPixel(8, 1);
        clearPixel(9, 1);
        clearPixel(10, 1);
        clearPixel(11, 1);
    }
}

void changeNineHour(uint8_t state) {
    if(state) {
        setPixel(12, 1);
        setPixel(13, 1);
        setPixel(14, 1);
        setPixel(15, 1);
    } else {
        clearPixel(12, 1);
        clearPixel(13, 1);
        clearPixel(14, 1);
        clearPixel(15, 1);
    }
}

void changeOclock(uint8_t state) {
    if(state) {
        setPixel(2, 0);
        setPixel(3, 0);
        setPixel(4, 0);
        setPixel(5, 0);
        setPixel(6, 0);
        setPixel(7, 0);
    } else {
        clearPixel(2, 0);
        clearPixel(3, 0);
        clearPixel(4, 0);
        clearPixel(5, 0);
        clearPixel(6, 0);
        clearPixel(7, 0);
    }
}

void changeAM(uint8_t state) {
    if(state) {
        setPixel(9, 0);
        setPixel(10, 0);
    } else {
        clearPixel(9, 0);
        clearPixel(10, 0);
    }
}

void changePM(uint8_t state) {
    if(state) {
        setPixel(11, 0);
        setPixel(12, 0);
    } else {
        clearPixel(11, 0);
        clearPixel(12, 0);
    }
}
