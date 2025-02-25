#ifndef USART_H_
#define USART_H_
/*Created on 29/05/2022 Lucas Canete*/

#define BAUD 9600UL
#define F_CPU 1000000UL

#include <avr/io.h>



#include <util/setbaud.h>
#include <math.h> //pow


void initUSART();

void transmitByte(uint8_t data);

uint8_t receiveByte(void);

void printString(const char myString []);
void printStringln(const char myString []);
void printBinaryByte(uint8_t byte);
void printByte(uint8_t byte);
void printUInt16(uint16_t num);
void printFloat(float number, uint8_t decimal_places = 2);
#endif
