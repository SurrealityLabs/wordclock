#include <Wire.h>
#include "TimerOne.h"
#include "RTC.h"
#include "DateTime.h"
#include "MatrixDisplay.h"
#include "debounce.h"

#define SecondsButtonPin 8
#define DownButtonPin 6
#define UpButtonPin 7
#define SetButtonPin 5

#define SETSTATE_HOUR 1
#define SETSTATE_MINUTE 2
#define SETSTATE_SECOND 3

#define SETTIMEOUT 45000

//RTC_DS1307 RTC;
RTC_MCP794XX RTC;
DebounceStorage SecondsButton, DownButton, UpButton, SetButton;

volatile uint8_t currentCol = 0;
uint8_t RunMode = 1;
unsigned long returnToRun = 0;
uint8_t setState = 0;
uint8_t setValue = 0;
DateTime setTime;

int brightnessAccumulator = 0;

extern void RenderTime(DateTime *currentTime);
extern void RenderTimeSeconds(DateTime *currentTime);
extern void RenderDigits(uint8_t dispNumber, uint8_t setQuadrant);

void displayTimerHandler(void);

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
    Serial.println("Starting");

    Wire.begin();
    RTC.begin();
    if (! RTC.isrunning()) {
        Serial.println("RTC is NOT running!");
        // following line sets the RTC to the date & time this sketch was compiled
        RTC.adjust(DateTime(__DATE__, __TIME__));
    }
    Serial.println("RTC setup");
    
    digitalWrite(10, LOW);
    
    Timer1.initialize(525);
    Timer1.attachInterrupt(displayTimerHandler);
    Timer1.pwm(10, 0, 0);
    //digitalWrite(10, LOW);
}

void loop() {
    DateTime now = RTC.now();
    
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
    Serial.print("Raw ight reading: ");
    Serial.println(lightReading);
    lightReading = constrain(lightReading, 0, 30);
    brightnessAccumulator -= (brightnessAccumulator >> 4);
    brightnessAccumulator += lightReading;
    int pwmOut = map(brightnessAccumulator, 0, 480, 950, 0);
    Timer1.setPwmDuty(10, 0);
    Serial.print("Light reading: ");
    Serial.print(lightReading);
    Serial.print(", PWM output: ");
    Serial.println(pwmOut);

    if(RunMode == 1) {
        // Throw away up and down status so as not to confuse us
        GetButtonChanged(&DownButton);
        GetButtonChanged(&UpButton);
        
        if(GetButtonState(&SetButton) && GetButtonChanged(&SetButton)) {
            RunMode = 2;
            setState = SETSTATE_HOUR;
            setTime = RTC.now();
            setValue = setTime.hour();
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
        if(setState == SETSTATE_HOUR) {
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
                setTime.setHour(setValue);
                setValue = setTime.minute();
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
                setTime.setMinute(setValue);
                setValue = setTime.second();
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
                setTime.setSecond(setValue);
                RTC.adjust(setTime);
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

