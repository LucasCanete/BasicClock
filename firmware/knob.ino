#include <defines.h>
#include <volumeBar.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <logo.h>
#include <Rotary.h>

#include "USB.h"
#include "USBHIDConsumerControl.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
VolumeBar volumeBar (&display,BAR_X,BAR_Y,VOLUME_BAR_WIDTH,VOLUME_BAR_HEIGHT,WHITE);
RotaryEncoder rotary(ROTARY_DT,ROTARY_CLK);
USBHIDConsumerControl ConsumerControl;

uint8_t volume = 0;

void setup() {
  delay(3000);
  //ConsumerControl.begin();
  //USB.begin();
  pinMode(ledPin,OUTPUT);
  rotary.init();
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();

  display.drawBitmap(
    0,
    0,
    hs_logoBitmap,
    128,
    32,
    WHITE
);
  display.display();
  delay(3000);
  display.clearDisplay();


}

void loop() {

  Direction dir = rotary.getDirection();
  if(dir == Direction::CLOCKWISE && volume <= 95) {
    Serial.println("CLOCKWISE");
    volume+=5;
    //ConsumerControl.press(CONSUMER_CONTROL_VOLUME_INCREMENT);
    //ConsumerControl.release();
  }
  else if (dir == Direction::COUNTERCLOCKWISE && volume >= 5) {
    Serial.println("COUNTERCLOCKWISE");
    volume-=5;
    //ConsumerControl.press(CONSUMER_CONTROL_VOLUME_DECREMENT);
    //ConsumerControl.release();
    };

  //Display Rectangle Box Around the display
  display.drawRect(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);


  volumeBar.setValue(volume);
  volumeBar.draw();

  // Display Text
  display.setTextSize(3);       // Set text size
  display.setTextColor(WHITE);  // Set text color
  display.setCursor(SCREEN_WIDTH*0.35, SCREEN_HEIGHT*0.1);     // Set cursor position
  display.println(volume);
  display.display();  // Display the content on the screen
  //delay(10);
  display.clearDisplay();  // Clear the screen




}
