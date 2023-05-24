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

Grid grid = Grid(3, 2, &tft);

void printHello(){
  Serial.println("Hello!");
}

Button helloPrinter = Button(
  "Click me!", //What text is on the label
  0, //Which row the label is in
  0, //Which collumn the label is in

  1, //Rows to take up
  1, //Collumns to take up

  ILI9341_WHITE, //Background color

  ILI9341_BLACK, //Color of text
  2, //Size of text
  true, //Bool if text should be centered or not

  &tft, //Pointer to tft object
  &grid, //Pointer to grid

  4, //Spacing between cells on the x axis
  4, //Spacing between cells on the y axis

  2, //Border width
  ILI9341_WHITE, //Border color

  10, //Radius, if you want the edges to be rounded. If high enough, can become circle.

  &printHello, //Pointer to function it should do
  &ts, //Pointer touchscreen object

  ILI9341_RED //Active color, the color it appears when clicked

);

void setup() {
  Serial.begin(9600);
  tft.begin();
  ts.begin();
  tft.setRotation(3);

  helloPrinter.draw();

}

void loop() {
  helloPrinter.checkTouch();
}
