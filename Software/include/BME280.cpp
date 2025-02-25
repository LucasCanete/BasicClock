#include <BME280.h>
#include <util/delay.h> //for soft reset

BME280::BME280(uint8_t addr){
    addr_ = addr;
    addr_w = (addr_ << 1); // 0 as LSB
    addr_r = (addr_ << 1) | 0x1; // 1 as LSB
}

/**
 * @brief initializes I2C
 * and proves if connection to sensor is available
 * @return uint8_t 
 */

uint8_t BME280::init() 
{
    initI2C();

    i2cStart();
    i2cSend(addr_w);
    i2cSend(0xD0); //register with chip ID

    i2cStart(); //restart
    i2cSend(addr_r); //read mode

    id = i2cReadNoAck();

    i2cStop();

    switch (id)
    {
    case 0x60:
        i2cStart();
        i2cSend(addr_w);
        i2cSend(BME280_REGISTER_SOFTRESET);
        i2cSend(0xB6);
        i2cStop();

        _delay_ms(10);

        i2cStart();
        i2cSend(addr_w);
        i2cSend(BME280_REGISTER_CONTROLHUMID);
        i2cSend(BME280_HUM_CONFIG);
        i2cStop();
        break;
    
    default:
        break;
    }

    
    i2cStart();
    i2cSend(addr_w);
    // write config for filter, standby-time and SPI-Mode (SPI off)
    i2cSend(BME280_REGISTER_CONFIG);
    i2cSend(BME280_CONFIG);

    // write config for humidity
    i2cSend(BME280_REGISTER_CONTROL);
    i2cSend((BME280_TEMP_CONFIG << 5)|(BME280_PRESS_CONFIG << 2)|(BME280_MODE_CONFIG));

    i2cStop();
    // wait for adjust configs
    _delay_ms(100);
 
    readCoefficients();
    return id; //must return 0x60 if bme was found

}

float BME280::readTemperature () 
{
    uint32_t adc_T; //raw reading data

    i2cStart();
    i2cSend(addr_w);
    i2cSend(BME280_REGISTER_TEMPDATA); //register with temp MSB

    i2cStart(); //restart
    i2cSend(addr_r);

    adc_T = i2cReadAck(); //receives temperature MSB
    adc_T <<= 8;
    adc_T |= i2cReadAck(); //autoincremented to next register 0xFB (temperature LSB)
    adc_T <<= 8;
    adc_T |= i2cReadNoAck(); //read XLSB (the 4 lsb are 0'S)

    i2cStop();
    int32_t var1, var2;

    adc_T >>= 4; //trig out the 4 zeros

    //Compensation formula from Datasheet
    var1 = ((((adc_T>>3) - ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T>>4) - ((int32_t)dig_T1)) * ((adc_T>>4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14; 
   
    t_fine = var1 + var2;
    float Temp = (t_fine * 5 + 128) >> 8;



    return Temp / 100;
}

uint8_t BME280::readHumidity() 
{
    int32_t adc_H;
    readTemperature(); //to actualize t_fine

    i2cStart();
    i2cSend(addr_w);
    i2cSend(BME280_REGISTER_HUMIDDATA); //register with temp MSB

    i2cStart(); //restart
    i2cSend(addr_r);
    adc_H = i2cReadAck(); //MSB
    adc_H <<= 8;
    adc_H |= i2cReadNoAck(); //LSB
    i2cStop();

       if (adc_H == 0x8000) // value in case humidity measurement was disabled
        return 11;

    int32_t v_x1_u32r;

    v_x1_u32r = (t_fine - ((int32_t)76800));

    v_x1_u32r = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) *
    v_x1_u32r)) + ((int32_t)16384)) >> 15) * (((((((v_x1_u32r *
        ((int32_t)dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)dig_H3)) >> 11) +
    ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)dig_H2) +
    8192) >> 14));

    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
    ((int32_t)dig_H1)) >> 4));

    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
    float h = (v_x1_u32r>>12);

    return h/1024.0;


}


float BME280::readPressure() 
{
    int32_t adc_P; //raw reading data

    readTemperature(); //for t_fine to be actualized

    i2cStart();
    i2cSend(addr_w);
    i2cSend(BME280_REGISTER_PRESSUREDATA); //register with temp MSB

    i2cStart(); //restart
    i2cSend(addr_r);

    adc_P = i2cReadAck(); //receives temperature MSB
    adc_P <<= 8;
    adc_P |= i2cReadAck(); //autoincremented to next register 0xF8 (temperature LSB)
    adc_P <<= 8;
    adc_P |= i2cReadNoAck(); //read XLSB (the 4 lsb are 0'S)

    i2cStop();
    adc_P >>= 4; //trig out the 4 zeroes

    int64_t var1, var2, p;


    //Compensation formula from Datasheet
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1*(int64_t)dig_P5)<<17);
    var2 = var2 + (((int64_t)dig_P4)<<35);
    var1 = ((var1 * var1 * (int64_t)dig_P3)>>8) + ((var1 * (int64_t)dig_P2)<<12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t)dig_P1)>>33;

    if (var1 == 0)
    {
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576 - adc_P;
    p = (((p<<31)-var2)*3125)/var1;
    var1 = (((int64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((int64_t)dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7)<<4);

    return (float)p/256ul;
}


uint8_t BME280::read1Byte(uint8_t reg){

    uint8_t value;

    i2cStart();
    i2cSend(addr_w);
    i2cSend(reg); 

    i2cStart(); //restart
    i2cSend(addr_r);

    value = i2cReadNoAck();
    i2cStop();

    return value;
}




uint16_t BME280::read2Bytes(uint8_t reg){

    uint16_t value;

    i2cStart();
    i2cSend(addr_w);
    i2cSend(reg); 

    i2cStart(); //restart
    i2cSend(addr_r);

    value = i2cReadAck();
    value <<= 8;
    value |= i2cReadNoAck(); //autoincrement and read next register

    i2cStop();

    return value;
}

uint16_t BME280::read16_swapped (uint8_t reg){
    uint16_t raw = read2Bytes(reg);
    return (raw >> 8) | (raw << 8);
}

int16_t BME280::read16S_swapped(uint8_t reg){
    return (int16_t)read16_swapped(reg);
}


void BME280::readCoefficients(){

    dig_T1 = read16_swapped(BME280_REGISTER_DIG_T1);
    dig_T2 = read16S_swapped(BME280_REGISTER_DIG_T2);
    dig_T3 = read16S_swapped(BME280_REGISTER_DIG_T3);

    dig_P1 = read16_swapped(BME280_REGISTER_DIG_P1);
    dig_P2 = read16S_swapped(BME280_REGISTER_DIG_P2);
    dig_P3 = read16S_swapped(BME280_REGISTER_DIG_P3);
    dig_P4 = read16S_swapped(BME280_REGISTER_DIG_P4);
    dig_P5 = read16S_swapped(BME280_REGISTER_DIG_P5);
    dig_P6 = read16S_swapped(BME280_REGISTER_DIG_P6);
    dig_P7 = read16S_swapped(BME280_REGISTER_DIG_P7);
    dig_P8 = read16S_swapped(BME280_REGISTER_DIG_P8);
    dig_P9 = read16S_swapped(BME280_REGISTER_DIG_P9);


    dig_H1 = read1Byte(BME280_REGISTER_DIG_H1);
    dig_H2 = read16S_swapped(BME280_REGISTER_DIG_H2);
    dig_H3 = read1Byte(BME280_REGISTER_DIG_H3);
    dig_H4 = (read1Byte(BME280_REGISTER_DIG_H4) << 4) | (read1Byte(BME280_REGISTER_DIG_H4+1) & 0xF);
    dig_H5 = (read1Byte(BME280_REGISTER_DIG_H5+1) << 4) | (read1Byte(BME280_REGISTER_DIG_H5) >> 4);
    dig_H6 = (int8_t)read1Byte(BME280_REGISTER_DIG_H6);


}
