#include "Arduino.h"
#include <Adafruit_ILI9341.h>
#include "Adafruit_GFX.h"

// GPIOs for SPI
#define SPI_MOSI      6
#define SPI_MISO      5
#define SPI_SCK       4

// GPIOs for TFT/TP
#define TFT_CS        8
#define TFT_DC         3
#define TP_CS         13
#define TP_IRQ        12

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
#define TFT_BLACK       0x0000
#define TFT_WHITE       0xFFFF



//TFT tft;

//-------------------------------------------------------------------------------------
void setup() {

    tft.begin();
    tft.setRotation(1); //landscape
    tft.setCursor(20,30);
    tft.print("Hello World!");
}
//-------------------------------------------------------------------------------------
void loop(void) {
    tft.fillScreen(TFT_BLACK);
    int boxWidth = 135;
    int boxHeight = 240;
    int boxX = (tft.width() - boxWidth) / 2;
    int boxY = (tft.height() - boxHeight) / 2;
    tft.drawRect(boxX, boxY, boxWidth, boxHeight, TFT_WHITE);

    // Display payload on TFT
    //tft.fillScreen(TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(10, 30);
    tft.print("G: ");
    tft.setCursor(10, 60);
    tft.print("33.6");

    int middleX = tft.width() / 2;
    tft.drawFastVLine(middleX, 0, tft.height(), TFT_WHITE);

    tft.setCursor(170, 30);
    tft.print("S: ");
    tft.setCursor(170, 60);
    tft.print("57.0");  // Replace randomValue with your actual random value
    delay(10000);
    
}
//-------------------------------------------------------------------------------------
