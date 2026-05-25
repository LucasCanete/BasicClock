#ifndef ROTARY_H
#define ROTARY_H

#include <Arduino.h>

// typedef enum {
// 	CLOCKWISE, //default value 0
// 	COUNTERCLOCKWISE, //default value 1
// 	NONE
// } rotary_direction_t;


enum class Direction {
  CLOCKWISE, //default value 0
	COUNTERCLOCKWISE, //default value 1
	NONE
};

class RotaryEncoder{
    public:
      RotaryEncoder(uint8_t DTPin, uint8_t CLKPin);

      void init();
      Direction getDirection();
      //uint8_t readButton() const;

    private:
      uint8_t currentclkstate;
      uint8_t lastclkstate = 0;

      uint8_t DTPin_;
      uint8_t CLKPin_;
   

};


#endif
