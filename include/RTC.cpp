#include <RTC.h>

DS3231::DS3231(uint8_t addr){
    addr_w = (addr<<1);
    addr_r = (addr<<1) | 0x1;
}

uint8_t DS3231::DecimalToBCD(uint8_t val){
    return ((val / 10 * 16) + (val % 10));
}

uint8_t DS3231::BCDToDecimal(uint8_t val){
    return (val / 16 * 10) + (val % 16);
}

uint8_t DS3231::writeByte(uint8_t reg, uint8_t val){
    i2cStart();

    i2cSend(addr_w);
    i2cSend(reg);
    i2cSend(val);

    i2cStop();

}

uint8_t DS3231::readByte(uint8_t reg){

    uint8_t val = 0;

    i2cStart();

    i2cSend(addr_w);
    i2cSend(reg);

    i2cStart(); //restart
    i2cSend(addr_r);

    val = i2cReadNoAck();

    i2cStop();

    return val;
}

uint8_t DS3231::init(){
    initI2C();
}

uint8_t DS3231::setSecond(uint8_t s){
    //first convert the value to BCD
    uint8_t s_bcd = DecimalToBCD(s);

    //Ensure that the 7th bit is always 0
    s_bcd &= (0b01111111);

    writeByte(DS3231_REGISTER_SECONDS, s_bcd);
}

uint8_t DS3231::second(){
    uint8_t s = readByte(DS3231_REGISTER_SECONDS);
    return BCDToDecimal(s);
}


uint8_t DS3231::setMinute(uint8_t m){
    //first convert the value to BCD
    uint8_t m_bcd = DecimalToBCD(m);

    //Ensure that the 7th bit is always 0
    m_bcd &= (0b01111111);

    writeByte(DS3231_REGISTER_MINUTES, m_bcd);
}

uint8_t DS3231::minute(){
    uint8_t m = readByte(DS3231_REGISTER_MINUTES);
    return BCDToDecimal(m);
}


uint8_t DS3231::setHour(uint8_t h){
    //first convert the value to BCD
    uint8_t h_bcd = DecimalToBCD(h);

    //Ensure that the 7th bit is always 0
    //Ensure that bit 6 is 0 for 24 hours mode
    h_bcd &= (0b00111111);

    writeByte(DS3231_REGISTER_HOURS, h_bcd);
}

uint8_t DS3231::hour(){
    uint8_t h = readByte(DS3231_REGISTER_HOURS);
    return BCDToDecimal(h);
}


uint8_t DS3231::setDate(uint8_t d){
    //first convert the value to BCD
    uint8_t d_bcd = DecimalToBCD(d);

    //Ensure that the 7th bit is always 0
    //6 bit is also 0
    d_bcd &= (0b00111111);

    writeByte(DS3231_REGISTER_DATE, d_bcd);


}

uint8_t DS3231::date(){
    uint8_t d = readByte(DS3231_REGISTER_DATE);
    return BCDToDecimal(d);
}


uint8_t DS3231::setMonth(uint8_t m){
    //first convert the value to BCD
    uint8_t m_bcd = DecimalToBCD(m);

    //Ensure bit 7 for Century is 1 when toggled
    m_bcd &= (0b10011111);

    writeByte(DS3231_REGISTER_MONTH_CENTURY, m_bcd);
}

uint8_t DS3231::month(){
    uint8_t m = readByte(DS3231_REGISTER_MONTH_CENTURY);
    return BCDToDecimal(m);
}


uint8_t DS3231::setYear(uint16_t y){
    //first convert the value to BCD
    uint8_t y_bcd = DecimalToBCD(y);

    writeByte(y_bcd);

}


uint8_t DS3231::year(){
    uint8_t y = readByte(DS3231_REGISTER_YEAR);
    return BCDToDecimal(y);
}
