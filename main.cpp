/*

 Lucas Canete - September 2024
 */

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <USART.h>
#include <BME280.h>

typedef enum {IDLE,MEASURE,SHOW,CONFIG} state_t;

int main(){

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

    state_t current_state = IDLE;
    DDRB |= (1<<PB1);

    while(1){
        PORTB ^= (1<<PB1);

        float temp = bme.readTemperature();
        float pressure = bme.readPressure()/100; //returns valid values
        uint8_t humidity = bme.readHumidity(); //returns 0




        printString("Temperature: ");
        printFloat(temp);
        //printByte((uint8_t)temp);
        printStringln(" ");

        printString("Pressure: ");
        printFloat(pressure);
        //printUInt16((uint16_t)pressure);
        //printByte((uint8_t)pressure);
        printStringln(" ");

        if (humidity > 0){
            printString("Humidity: ");
            printByte((uint8_t)humidity);
            printStringln(" ");
            printStringln(" ");

        }

         printStringln(" ");
        _delay_ms(1000);

        #if 0 
        switch(current_state){
            case IDLE:
                break;
            case MEASURE:

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
