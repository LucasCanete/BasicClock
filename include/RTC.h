/*
 Cpp ibrary for rtc ds3231
 09.2024
 Lucas Canete
 */

#ifndef RTC_H
#define RTC_H

#include<I2C.h>

#define DS3231_ADDR 0x68

#define DS3231_REGISTER_SECONDS 0
#define DS3231_REGISTER_MINUTES 0x1
#define DS3231_REGISTER_HOURS 0x2
#define DS3231_REGISTER_DAY 0x3
#define DS3231_REGISTER_DATE 0x4
#define DS3231_REGISTER_MONTH_CENTURY 0x5
#define DS3231_REGISTER_YEAR 0x6

class DS3231 {
public:
    DS3231(uint8_t addr = DS3231_ADDR);
    uint8_t init();


    uint8_t setSecond(uint8_t s);
    uint8_t second();


    uint8_t setMinute(uint8_t m);
    uint8_t minute();


    uint8_t setHour(uint8_t h);
    uint8_t hour();


    uint8_t setDate(uint8_t d);
    uint8_t date();


    uint8_t setMonth(uint8_t m);
    uint8_t month();


    uint8_t setYear(uint16_t y);
    uint8_t year();

private:
    uint8_t writeByte(uint8_t reg, uint8_t val);
    uint8_t readByte(uint8_t reg);

    uint8_t DecimalToBCD(uint8_t val);
    uint8_t BCDToDecimal(uint8_t val);

    uint8_t addr_w = 0;
    uint8_t addr_r = 0;

};

#endif
