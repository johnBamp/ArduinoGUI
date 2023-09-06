#ifndef BASE_H
#define BASE_H

#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <Adafruit_FT6206.h>
#include <WiFi.h>
#include "time.h"
#include "arduinoGUI.h"
#include <Fonts/FreeSans12pt7b.h> 
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <vector>
#include <SdFat.h>
#include "RTClib.h"

class BaseApp {
  public:
    virtual void appSetup(){};
    virtual void appLoop(){};

    virtual void icon(GFXcanvas16& canvas){};
};

#endif
