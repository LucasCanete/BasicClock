 

##########------------------------------------------------------##########
##########              Project-specific Details                ##########
##########    Check these every time you start a new project    ##########
##########------------------------------------------------------##########
#atmega168 or attiny85 or atmega328p
MCU   = atmega328p
F_CPU = 1000000UL
BAUD  = 9600UL



ifeq ($(STD),)
STD=c++17
endif

BUILD_DIR=./build
LIB_DIR= /home/lucas/Embedded/AVR_projects/NightStandClock
LIBDIR = /home/lucas/Embedded/AVR_projects/NightStandClock/include

ifeq ($(INC),)
INCLUDES= -I$(LIB_DIR)/include
else
INCLUDES= -I$(INC)
endif




## Also try BAUD = 19200 or 38400 if you're feeling lucky.

## A directory for common include files and the simple USART library.
## If you move either the current folder or the Library folder, you'll
##  need to change this path to match.
#LIBDIR = /home/lucas/Embedded/AVR_projects/test_cpp/include

##########------------------------------------------------------##########
##########                 Programmer Defaults                  ##########
##########          Set up once, then forget about it           ##########
##########        (Can override.  See bottom of file.)          ##########
##########------------------------------------------------------##########

PROGRAMMER_TYPE = usbasp
# extra arguments to avrdude: baud rate, chip type, -F flag, etc.
PROGRAMMER_ARGS =

##########------------------------------------------------------##########
##########                  Program Locations                   ##########
##########     Won't need to change if they're in your PATH     ##########
##########------------------------------------------------------##########

CC = avr-gcc
CXX=avr-g++
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
AVRSIZE = avr-size
AVRDUDE = avrdude

##########------------------------------------------------------##########
##########                   Makefile Magic!                    ##########
##########         Summary:                                     ##########
##########             We want a .hex file                      ##########
##########        Compile source files into .elf                ##########
##########        Convert .elf file into .hex                   ##########
##########        You shouldn't need to edit below.             ##########
##########------------------------------------------------------##########

## The name of your project (without the .c)
# TARGET = blinkLED
## Or name it automatically after the enclosing directory
TARGET = $(lastword $(subst /, ,$(CURDIR)))

# Object files: will find all .c/.h files in current directory
#  and in LIBDIR.  If you have any other (sub-)directories with code,
#  you can add them in to SOURCES below in the wildcard statement.
SOURCES=$(wildcard *.cpp $(LIBDIR)/*.cpp)
#OBJECTS=$(SOURCES:.cpp=.o)
#HEADERS=$(SOURCES:.cpp=.h)
VPATH=.:$(LIBDIR)
#OBJECTS=$(addprefix $(BUILD_DIR)/,$(notdir $(SOURCES:%=%.o))) # only works when everything is in the same repo, include repo must be empty

OBJECTS=$(addprefix $(BUILD_DIR)/,$(notdir $(SOURCES:%=%.o)))

WARNFLAGS=-Wall -Wextra -pedantic

ifeq ($(STD),c++20)
WARNFLAGS+=-Wno-volatile
endif

CXXFLAGS=-std=$(STD) -O2 $(WARNFLAGS) -I. -I$(LIBDIR) -fno-exceptions -fno-rtti -fno-unwind-tables -fno-threadsafe-statics -Wshadow -Wcast-qual -Wpointer-arith -Wundef
LDFLAGS=-lm



all: $(TARGET).hex

hex: $(TARGET).hex


$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -j .data -j .text $(TARGET).elf $(TARGET).hex

$(TARGET).elf: $(OBJECTS)
	avr-g++ $(LDFLAGS) -mmcu=$(MCU) $(OBJECTS) -o $(TARGET).elf

$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	avr-g++ -c $(CXXFLAGS) -mmcu=$(MCU) $(INCLUDES) $< -o $@

$(BUILD_DIR)/%.cc.o: %.cc
	@mkdir -p $(BUILD_DIR)
	avr-g++ -c $(CXXFLAGS) -mmcu=$(MCU) $(INCLUDES) $< -o $@

size: $(TARGET).elf
	avr-objdump -Pmem-usage $(TARGET).elf

program: $(TARGET).hex
	avrdude -p$(MCU) $(AVRDUDE_FLAGS) -c$(PROGRAMMER) -Uflash:w:$(TARGET).hex:a

clean:
	rm -rf $(BUILD_DIR)/*.o
	rm -rf $(BUILD_DIR)/*.elf
	rm -rf $(BUILD_DIR)/*.hex

##########------------------------------------------------------##########
##########              Programmer-specific details             ##########
##########           Flashing code to AVR using avrdude         ##########
##########------------------------------------------------------##########

flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -p $(MCU) $(PROGRAMMER_ARGS) -U flash:w:$<

## An alias
program: flash

flash_eeprom: $(TARGET).eeprom
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -p $(MCU) $(PROGRAMMER_ARGS) -U eeprom:w:$<

avrdude_terminal:
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -p $(MCU) $(PROGRAMMER_ARGS) -nt

## If you've got multiple programmers that you use,
## you can define them here so that it's easy to switch.
## To invoke, use something like `make flash_arduinoISP`
flash_usbtiny: PROGRAMMER_TYPE = usbatiny
flash_usbtiny: PROGRAMMER_ARGS =  # USBTiny works with no further arguments
flash_usbtiny: flash

flash_usbasp: PROGRAMMER_TYPE = usbasp
flash_usbasp: PROGRAMMER_ARGS =  # USBasp works with no further arguments
flash_usbasp: flash

flash_arduinoISP: PROGRAMMER_TYPE = avrisp
flash_arduinoISP: PROGRAMMER_ARGS = -b 19200 -P /dev/ttyACM0
## (for windows) flash_arduinoISP: PROGRAMMER_ARGS = -b 19200 -P com5
flash_arduinoISP: flash

flash_109: PROGRAMMER_TYPE = avr109
flash_109: PROGRAMMER_ARGS = -b 9600 -P /dev/ttyUSB0
flash_109: flash

##########------------------------------------------------------##########
##########       Fuse settings and suitable defaults            ##########
##########------------------------------------------------------##########

## Mega 48, 88, 168, 328 default values
LFUSE = 0x62
HFUSE = 0xdf
EFUSE = 0x00

## Generic
FUSE_STRING = -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m

fuses:
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -p $(MCU) \
	           $(PROGRAMMER_ARGS) $(FUSE_STRING)
show_fuses:
	$(AVRDUDE) -c $(PROGRAMMER_TYPE) -p $(MCU) $(PROGRAMMER_ARGS) -nv

## Called with no extra definitions, sets to defaults
set_default_fuses:  FUSE_STRING = -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m
set_default_fuses:  fuses

## Set the fuse byte for full-speed mode
## Note: can also be set in firmware for modern chips
set_fast_fuse: LFUSE = 0xE2
set_fast_fuse: FUSE_STRING = -U lfuse:w:$(LFUSE):m
set_fast_fuse: fuses

## Set the EESAVE fuse byte to preserve EEPROM across flashes
set_eeprom_save_fuse: HFUSE = 0xD7
set_eeprom_save_fuse: FUSE_STRING = -U hfuse:w:$(HFUSE):m
set_eeprom_save_fuse: fuses

## Clear the EESAVE fuse byte
clear_eeprom_save_fuse: FUSE_STRING = -U hfuse:w:$(HFUSE):m
clear_eeprom_save_fuse: fuses
