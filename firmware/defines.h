#ifndef DEFINES_H
#define DEFINES_H

//Board: XIAO ESP32S3
constexpr uint8_t ledPin = 21;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define BAR_X SCREEN_WIDTH*0.10/2
#define BAR_Y SCREEN_HEIGHT*0.80

#define VOLUME_BAR_HEIGHT 5
#define VOLUME_BAR_WIDTH SCREEN_WIDTH*0.90

#define ROTARY_DT 1
#define ROTARY_CLK 2


#endif
