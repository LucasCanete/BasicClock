#include "esp32-hal-gpio.h"
#include "Rotary.h"


RotaryEncoder::RotaryEncoder(uint8_t DTPin, uint8_t CLKPin):DTPin_(DTPin), CLKPin_(CLKPin){

}


void RotaryEncoder::init(){

  pinMode(CLKPin_,INPUT);
  pinMode(DTPin_,INPUT);
  lastclkstate = digitalRead(CLKPin_);
}

//Rising EDGE
Direction RotaryEncoder::getDirection() {
  //currentclkstate = *CLKPort_ & (1 << CLKPin_);
  Direction direction = Direction::NONE;

  currentclkstate = digitalRead(CLKPin_);

  if (currentclkstate != lastclkstate){
    //uint8_t DT_State = *DTPort_ & (1 << DTPin_);
    uint8_t DT_State = digitalRead(DTPin_);

    if (DT_State != currentclkstate) direction = Direction::CLOCKWISE; //change if eventually wrong

    else { direction = Direction::COUNTERCLOCKWISE;} //change if eventually wrong
  }
  lastclkstate = currentclkstate;
  return direction;
}



