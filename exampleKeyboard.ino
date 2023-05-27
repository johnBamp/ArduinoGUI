#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <Adafruit_FT6206.h>
#include <WiFi.h>
#include "time.h"
#include "arduinoGUI.h"

#define TFT_CHIP_SLCT 32
#define TFT_DATA_CMD 33

#define baud_rate 9600

Adafruit_FT6206 ts = Adafruit_FT6206();
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CHIP_SLCT, TFT_DATA_CMD);

Grid keyGrid = Grid(5, 10, &tft);

Keyboard keys = Keyboard(&tft, &keyGrid, &ts);

void setup() {
  Serial.begin(9600);
  tft.begin();
  ts.begin();
  tft.setRotation(3);

  keys.draw();

}

void loop() {
  keys.readKeys();
}
