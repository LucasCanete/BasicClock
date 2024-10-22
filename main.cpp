/*

 Lucas Canete - September 2024
 */

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <USART.h>
#include <BME280.h>
#include <RTC.h>
/*
 Button 1:PD2
 Button 2:PD3
 Button 3:PD4
 */

//Falling Edge PD2
ISR(INT0_vect){}
//Fallin Edge PD3
ISR(INT0_vect){}
typedef enum {IDLE,MEASURE,SHOW,CONFIG} state_t;

int main(){
    //Initialize PD2 and PD3 as interrupts
    EIMSK = (1<<INT1) | (1<<INT0):

    //Falling edge of PD2 and PD2 activates interrupts
    EICRA = (1<<ISC11) | (1<<ISC01);

    //Activate int. Flags from INT0 and INT1


    initUSART();

    BME280 bme(0x76);
    uint8_t id = bme.init();

    if (id == 0x60){
        printStringln("Success: Chip ID is 0x60!");
    }
    else {
        printStringln("Error: Chip not detected");
    }

    float temp, pressure;
    uint8_t humidity;

    DS3231 rtc;
    rtc.setSecond(0);
    rtc.setMinute(30);
    rtc.setHour(22);
    uint8_t hour, minute;

    state_t current_state = IDLE;
    DDRB |= (1<<PB1);

    while(1){
        PORTB ^= (1<<PB1);

        temp = bme.readTemperature();
        pressure = bme.readPressure()/100; //returns valid values
        humidity = bme.readHumidity(); //returns 0

        uint8_t second = rtc.second();
        minute = rtc. minute();
        hour = rtc.hour();

        printString("Hour: ");
        printByte(hour);
        printString(" Min: ");
        printByte(minute);
        printString(" Sec: ");
        printByte(second);
        printStringln(" ");

        printString("Temperature: ");
        printFloat(temp);
        //printByte((uint8_t)temp);
        printStringln(" ");

        printString("Pressure: ");
        printFloat(pressure);
        //printUInt16((uint16_t)pressure);
        //printByte((uint8_t)pressure);
        printStringln(" ");


        printString("Humidity: ");
        printByte((uint8_t)humidity);
        printStringln(" ");
        printStringln(" ");



         printStringln(" ");
        _delay_ms(1000);

        #if 0 
        switch(current_state){
            case IDLE:
                break;
            case MEASURE:

                minute = rtc. minute();
                hour = rtc.hour();

                temp = bme.readTemperature();
                pressure = bme.readPressure()/100;
                humidity = bme.readHumidity();

                break;
            case SHOW:
                break;
            case CONFIG:
                break;

        }
        #endif

    }
    return 0;
}
