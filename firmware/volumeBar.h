/*
  Bar that can be displayed to represent, volume, bright intensity, etc
*/
#pragma once

#ifndef VOLUMEBAR_H
#define VOLUMEBAR_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class VolumeBar {
  public:
    VolumeBar(Adafruit_SSD1306 *display_inst,int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t maxValue = 100);

    //max value percentage
    void setMaxValue(uint16_t v);
    //max value percentage
    uint16_t getMaxValue() const;

    //value percentage
    void setValue(uint16_t v);
    //value percentage
    uint16_t getValue() const;

    void setXposition(int16_t x);
    int16_t getXposition() const;

    void setYposition(int16_t y);
    int16_t getYposition() const;

    int16_t getWidth() const;
    int16_t getHeight() const;

    void fillBar(int16_t w, int16_t h) const;
    void draw() const;
    void display() const;


  private:

    uint16_t maxValue;
    uint16_t value;

    int16_t posX;
    int16_t posY;

    int16_t height;
    int16_t width;

    uint16_t color;

    Adafruit_SSD1306 *oledDisplay;

    void fillBar() const;


};


#endif 