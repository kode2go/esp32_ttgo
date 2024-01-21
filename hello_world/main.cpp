/*
Reference: https://www.youtube.com/watch?v=57JWlSQ6-v4
*/

#include <Arduino.h>
#include <TFT_eSPI.h>


TFT_eSPI tft=TFT_eSPI();


void setup() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.drawString("Hello World",30,30,2);  

}

void loop() { 
 
}

