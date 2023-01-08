#include <Wire.h>
#include <Timezone.h>           // https://github.com/JChristensen/Timezone - GPL License
#include "TimerOne.h"
#include "RTC.h"
#include "DateTime.h"
#include "MatrixDisplay.h"
#include "debounce.h"
#include <avr/pgmspace.h>

#define SecondsButtonPin 8
#define DownButtonPin 6
#define UpButtonPin 7
#define SetButtonPin 5

#define SETSTATE_YEAR 1
#define SETSTATE_MONTH 2
#define SETSTATE_DAY 3
#define SETSTATE_HOUR 4
#define SETSTATE_MINUTE 5
#define SETSTATE_SECOND 6

#define SETTIMEOUT 45000

//RTC_DS1307 RTC;
RTC_MCP794XX RTC;
DebounceStorage SecondsButton, DownButton, UpButton, SetButton;

volatile uint8_t currentCol = 0;
uint8_t RunMode = 1;
unsigned long returnToRun = 0;
uint8_t setState = 0;
uint16_t setValue = 0;
uint32_t tmpTime = 0;
myDateTime timeToSet;

// US Eastern Time Zone (New York, Detroit)
TimeChangeRule myDST = {"EDT", Second, Sun, Mar, 2, -240};    // Daylight time = UTC - 4 hours
TimeChangeRule mySTD = {"EST", First, Sun, Nov, 2, -300};     // Standard time = UTC - 5 hours
Timezone myTZ(myDST, mySTD);

const static uint8_t daysInMonth [] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 };

int brightnessAccumulator = 0;

extern void RenderTime(myDateTime *currentTime);
extern void RenderTimeSeconds(myDateTime *currentTime);
extern void RenderDigits(uint8_t dispNumber, uint8_t setQuadrant);

void displayTimerHandler(void);

uint8_t getDaysInMonth(uint8_t m, uint16_t y)
{
  bool leap = false;

  if (y % 400 == 0)
  {
    leap = true;
  }
  else if (y % 100 == 0)
  {
    leap = false;
  }
  else if (y % 4 == 0)
  {
    leap = true;
  }
  uint8_t daysPerMonth = pgm_read_byte(daysInMonth + m - 1);
  if (leap && m == 2)
      ++daysPerMonth;
}

void setup() {
    ClearDisplayArray();
    SetupFastShiftOut();
    
    SecondsButton = InitializeDebounce(5, 5, 0);
    DownButton = InitializeDebounce(5, 5, 500);
    UpButton = InitializeDebounce(5, 5, 500);
    SetButton = InitializeDebounce(5, 5, 0);
    
    pinMode(SecondsButtonPin, INPUT);
    pinMode(DownButtonPin, INPUT);
    pinMode(UpButtonPin, INPUT);
    pinMode(SetButtonPin, INPUT);
    
    Serial.begin(38400);
    Serial.println(F("Starting"));

    Wire.begin();
    RTC.begin();
    if (! RTC.isrunning()) {
        Serial.println(F("RTC is NOT running!"));
        // following line sets the RTC to the date & time this sketch was compiled
        RTC.adjust(myDateTime(__DATE__, __TIME__));
    }
    Serial.println(F("RTC setup"));
    
    digitalWrite(10, LOW);
    
    Timer1.initialize(525);
    Timer1.attachInterrupt(displayTimerHandler);
    Timer1.pwm(10, 0, 0);
    //digitalWrite(10, LOW);
}

void loop() {
    myDateTime now = RTC.now();
    tmpTime = now.unixtime();
    tmpTime = myTZ.toLocal(tmpTime);
    now.setUnixtime(tmpTime);
    
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
    
    int lightReading = analogRead(0);
    Serial.print(F("Raw light reading: "));
    Serial.println(lightReading);
    lightReading = constrain(lightReading, 0, 30);
    brightnessAccumulator -= (brightnessAccumulator >> 4);
    brightnessAccumulator += lightReading;
    int pwmOut = map(brightnessAccumulator, 0, 480, 950, 0);
    Timer1.setPwmDuty(10, 0);
    Serial.print(F("Light reading: "));
    Serial.print(lightReading);
    Serial.print(F(", PWM output: "));
    Serial.println(pwmOut);

    if(RunMode == 1) {
        // Throw away up and down status so as not to confuse us
        GetButtonChanged(&DownButton);
        GetButtonChanged(&UpButton);
        
        if(GetButtonState(&SetButton) && GetButtonChanged(&SetButton)) {
            RunMode = 2;
            setState = SETSTATE_YEAR;
            timeToSet = RTC.now();
            tmpTime = timeToSet.unixtime();
            tmpTime = myTZ.toLocal(tmpTime);
            timeToSet.setUnixtime(tmpTime);
            setValue = timeToSet.year();
            returnToRun = millis();
        }
        
        if(!GetButtonState(&SecondsButton)) {
            RenderTime(&now);
        } else {
            RenderTimeSeconds(&now);
        }
    } else if(RunMode == 2) {
        if(GetButtonState(&SecondsButton) && GetButtonChanged(&SecondsButton)) {
            RunMode = 1;
        }
        if (setState == SETSTATE_YEAR) {
            RenderDigits(setValue - 2000, 4);

            if(GetButtonStateRepeat(&UpButton)) {
                setValue++;
                if(setValue >= 2100) setValue = 2000;
                returnToRun = millis();
            }

            if(GetButtonStateRepeat(&DownButton)) {
                if(setValue == 2000) {
                    setValue = 2099;
                } else {
                    setValue--;
                }
                returnToRun = millis();
            }

            if(GetButtonState(&SetButton) && GetButtonChanged(&SetButton)) {
                setState = SETSTATE_MONTH;
                timeToSet.setYear(setValue);
                setValue = timeToSet.month();
                returnToRun = millis();
            }
        }
        else if(setState == SETSTATE_MONTH) {
            RenderDigits(setValue, 5);
            
            if(GetButtonStateRepeat(&UpButton)) {
                setValue++;
                if(setValue >= 13) setValue = 1;
                returnToRun = millis();
            }

            if(GetButtonStateRepeat(&DownButton)) {
                if(setValue == 0) {
                    setValue = 12;
                } else {
                    setValue--;
                }
                returnToRun = millis();
            }

            if(GetButtonState(&SetButton) && GetButtonChanged(&SetButton)) {
                setState = SETSTATE_DAY;
                timeToSet.setMonth(setValue);
                setValue = timeToSet.day();
                returnToRun = millis();
            }
        }
        else if(setState == SETSTATE_DAY) {
            RenderDigits(setValue, 6);
            
            if(GetButtonStateRepeat(&UpButton)) {
                setValue++;
                if(setValue >= (getDaysInMonth(timeToSet.month(), timeToSet.year()) + 1)) setValue = 1;
                returnToRun = millis();
            }

            if(GetButtonStateRepeat(&DownButton)) {
                if(setValue == 0) {
                    setValue = getDaysInMonth(timeToSet.month(), timeToSet.year());
                } else {
                    setValue--;
                }
                returnToRun = millis();
            }

            if(GetButtonState(&SetButton) && GetButtonChanged(&SetButton)) {
                setState = SETSTATE_HOUR;
                timeToSet.setDay(setValue);
                setValue = timeToSet.hour();
                returnToRun = millis();
            }
        }
        else if(setState == SETSTATE_HOUR) {
            RenderDigits(setValue, 1);
            
            if(GetButtonStateRepeat(&UpButton)) {
                setValue++;
                if(setValue >= 24) setValue = 0;
                returnToRun = millis();
            }

            if(GetButtonStateRepeat(&DownButton)) {
                if(setValue == 0) {
                    setValue = 23;
                } else {
                    setValue--;
                }
                returnToRun = millis();
            }

            if(GetButtonState(&SetButton) && GetButtonChanged(&SetButton)) {
                setState = SETSTATE_MINUTE;
                timeToSet.setHour(setValue);
                setValue = timeToSet.minute();
                returnToRun = millis();
            }
        } else if(setState == SETSTATE_MINUTE) {
            RenderDigits(setValue, 2);
            
            if(GetButtonStateRepeat(&UpButton)) {
                setValue++;
                if(setValue >= 60) setValue = 0;
                returnToRun = millis();
            }

            if(GetButtonStateRepeat(&DownButton)) {
                if(setValue == 0) {
                    setValue = 59;
                } else {
                    setValue--;
                }
                returnToRun = millis();
            }

            if(GetButtonState(&SetButton) && GetButtonChanged(&SetButton)) {
                setState = SETSTATE_SECOND;
                timeToSet.setMinute(setValue);
                setValue = timeToSet.second();
                returnToRun = millis();
            }
        } else if(setState == SETSTATE_SECOND) {
            RenderDigits(setValue, 3);
            
            if(GetButtonStateRepeat(&UpButton)) {
                setValue++;
                if(setValue >= 60) setValue = 0;
                returnToRun = millis();
            }

            if(GetButtonStateRepeat(&DownButton)) {
                if(setValue == 0) {
                    setValue = 59;
                } else {
                    setValue--;
                }
                returnToRun = millis();
            }

            if(GetButtonState(&SetButton) && GetButtonChanged(&SetButton)) {
                setState = SETSTATE_HOUR;
                RunMode = 1;
                timeToSet.setSecond(setValue);

                tmpTime = timeToSet.unixtime();
                tmpTime = myTZ.toUTC(tmpTime);
                timeToSet.setUnixtime(tmpTime);
                RTC.adjust(timeToSet);
            }
        }
        if ( ( millis() - returnToRun ) > SETTIMEOUT ) {
            RunMode = 1;
        }
    } else {
        RunMode = 1;
    }
    
    delay(100);
}

void displayTimerHandler() {
    if(digitalRead(SecondsButtonPin)) {
        RegisterDebounceLow(&SecondsButton);
    } else {
        RegisterDebounceHigh(&SecondsButton);
    }
    
    if(digitalRead(DownButtonPin)) {
        RegisterDebounceLow(&DownButton);
    } else {
        RegisterDebounceHigh(&DownButton);
    }
    
    if(digitalRead(UpButtonPin)) {
        RegisterDebounceLow(&UpButton);
    } else {
        RegisterDebounceHigh(&UpButton);
    }
    
    if(digitalRead(SetButtonPin)) {
        RegisterDebounceLow(&SetButton);
    } else {
        RegisterDebounceHigh(&SetButton);
    }
    
    OutputDisplayColumn(currentCol++);
    currentCol &= 0x0F;
}

