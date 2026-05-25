#include "volumeBar.h"

VolumeBar::VolumeBar(Adafruit_SSD1306 *display_inst, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t maxValue)
  :posX(x),posY(y),width(w),height(h),color(color),maxValue(maxValue),value(0),oledDisplay(display_inst) {};

void VolumeBar::setMaxValue(uint16_t v){
  maxValue = v;
};

uint16_t VolumeBar::getMaxValue() const{
  return maxValue;
};

void VolumeBar::setValue(uint16_t v){
  value = v > maxValue ? 0 : v;
};

uint16_t VolumeBar::getValue() const {
  return value;
};

void VolumeBar::setXposition(int16_t x) {
  posX = x > width ? 0 : x;
};

int16_t VolumeBar::getXposition() const{
  return posX;
};

void VolumeBar::setYposition(int16_t y){
  posY = y > height ? 0 : y;
};

int16_t VolumeBar::getYposition() const {
  return posY;
};

int16_t VolumeBar::getWidth() const {
  return width;
}

int16_t VolumeBar::getHeight() const {
  return height;
}

void VolumeBar::fillBar(int16_t w, int16_t h) const{
  oledDisplay->fillRect(posX, posY, w, h, color);
}

void VolumeBar::draw() const{
  oledDisplay->drawRect(posX,posY, width, height, color);

  bool isHorizontalBar = width > height ? true : false;
  
  if(isHorizontalBar){
    //Dreisatz
    int16_t newWidth = value * width / 100;
    fillBar(newWidth,height);
  }
  else{
    int16_t newHeight = value * height / 100;
    fillBar(width,newHeight);
  }


};

void VolumeBar::display() const{
  oledDisplay->display();
};