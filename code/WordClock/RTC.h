#include <Arduino.h>
#include "DateTime.h"
#include "Wire.h"

#ifndef SURREALITYLABS_RTC
#define SURREALITYLABS_RTC

#define MCP794XX_ADDRESS 0x6F
#define DS1307_ADDRESS 0x68

class RTC {
    public:
        virtual uint8_t begin(void) = 0;
        virtual void adjust(const myDateTime& dt) = 0;
        virtual uint8_t isrunning(void) = 0;
        virtual myDateTime now() = 0;
};

class RTC_MCP794XX : public RTC {
    public:
        uint8_t begin(void);
        void adjust(const myDateTime& dt);
        uint8_t isrunning(void);
        myDateTime now();
};

class RTC_DS1307 : public RTC {
    public:
        uint8_t begin(void);
        void adjust(const myDateTime& dt);
        uint8_t isrunning(void);
        myDateTime now();
};
#endif
