#ifndef CLOCKAPP_H
#define CLOCKAPP_H

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
#include "base.h"

class ClockApp : public BaseApp {
  private:
    Adafruit_ILI9341* tft;
    Adafruit_FT6206* ts; 
    RTC_PCF8523* rtc;
    Widget* timeLabel;  // This is a pointer

  public:
    ClockApp(Adafruit_ILI9341* tft, Adafruit_FT6206* ts, RTC_PCF8523* rtc)
      : tft(tft), ts(ts), rtc(rtc) {   
        timeLabel = new Widget(0, 0, 320, 240, ILI9341_BLACK, ts, tft);  // Create new Widget and assign its pointer
    }

    ~ClockApp() {
        delete timeLabel;  // Clean up dynamically allocated memory
    }

    void appSetup() override {
        // Your setup code
    }

    void appLoop() override {
        String dateStr = "";  // Declare dateStr
        DateTime now = rtc->now();

        if(now.hour() > 12) { 
            dateStr += String(now.hour() - 12, DEC);
        } else if(now.hour() == 0) { 
            dateStr += "12";
        } else {
            dateStr += String(now.hour(), DEC);
        }

        dateStr += ":";

        if(now.minute() < 10) {
            dateStr += "0"; 
        }
        dateStr += String(now.minute(), DEC);

        timeLabel->setText(dateStr, 2, true, 0, &FreeSans12pt7b);  // use timeLabel, not widget
        timeLabel->draw();  // same here
    }

    void icon(GFXcanvas16& canvas) override {
      int centerX = canvas.width() / 2;
      int centerY = canvas.height() / 2;
      canvas.drawCircle(centerX, centerY, 20, ILI9341_WHITE);
      canvas.drawCircle(centerX, centerY, 19, ILI9341_WHITE);

      canvas.fillCircle(centerX, centerY, 2, ILI9341_WHITE);
      canvas.drawLine(centerX, centerY, centerX + 5, centerY + 5, ILI9341_WHITE);
      canvas.drawLine(centerX, centerY, centerX, centerY - 10, ILI9341_WHITE);
    };
};

#endif
