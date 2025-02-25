
#include "USART.h"




void initUSART(){
	//set Baudrate
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	#if USE_2X
	UCSR0A |= (1 << U2X0);
	#else
	UCSR0A &= ~(1 << U2X0);
	#endif
	
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8 data bits, 1 stop bit */
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void transmitByte(uint8_t data){
	
	while(!(UCSR0A & (1<<UDRE0)));/* Wait for empty transmit buffer */
	UDR0 = data;/* send data */
}

uint8_t receiveByte(void){
	
	while(!(UCSR0A & (1<<RXC0)));/* Wait for data to be received */
	return UDR0;
}
void printString(const char myString []){
	uint8_t i = 0;
	while (myString[i]) {
	transmitByte(myString[i]);
	i++;
	}
}

void printStringln(const char myString []){
	uint8_t i = 0;
	
	while (myString[i]) {
	transmitByte(myString[i]);
	i++;
	}
	transmitByte('\n');
	transmitByte('\r');

}

void printBinaryByte(uint8_t byte){
	/* Prints out a byte as a series of 1's and 0's */
	uint8_t bit;
	for (bit=7; bit < 255; bit--){
		if ( bit_is_set(byte, bit) )
		transmitByte('1');
		else
		transmitByte('0');
	}
}

void printByte(uint8_t byte) {
	/* Converts a byte to a string of decimal text, sends it */
	transmitByte('0' + (byte / 100));                        /* Hundreds */
	transmitByte('0' + ((byte / 10) % 10));                      /* Tens */
	transmitByte('0' + (byte % 10));                             /* Ones */
}

void printUInt16(uint16_t num) {
    /* Converts a uint16_t to a string of decimal text, sends it */
    
    // Handle numbers less than 10000 (avoid leading zeros)
    bool leading_zero = true;

    // Transmit digits starting from ten-thousands place
    if (num >= 10000) {
        transmitByte('0' + (num / 10000));        /* Ten-thousands */
        num %= 10000;
        leading_zero = false;
    }

    if (!leading_zero || num >= 1000) {
        transmitByte('0' + (num / 1000));         /* Thousands */
        num %= 1000;
        leading_zero = false;
    }

    if (!leading_zero || num >= 100) {
        transmitByte('0' + (num / 100));          /* Hundreds */
        num %= 100;
        leading_zero = false;
    }

    if (!leading_zero || num >= 10) {
        transmitByte('0' + (num / 10));           /* Tens */
        num %= 10;
        leading_zero = false;
    }

    // Always print the ones place
    transmitByte('0' + num);                      /* Ones */
}

void printFloat(float number, uint8_t decimal_places) {
    // Handle negative numbers
    if (number < 0) {
        transmitByte('-');
        number = -number;
    }

    // Extract and print the integer part
    uint16_t int_part = (uint16_t)number;
    printUInt16(int_part);

    // Print the decimal point
    transmitByte('.');

    // Extract and print the fractional part
    float fraction = number - (float)int_part;

    // Multiply the fractional part by 10^decimal_places to shift decimals into the integer range
    uint32_t fractional_part = (uint32_t)(fraction * (float)pow(10, decimal_places));

    // Print fractional part with appropriate decimal places
    printUInt16(fractional_part);
}

