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
#include "RTClib.h"

class clockApp {
  private:

    static void drawClockBackground(GFXcanvas16* canvas){
      int n, i, cx = canvas->width()  / 2 - 1, cy = canvas->height() / 2 - 1;
      n = min(cx, cy);
      for(i=0; i<n; i+=5) {
        canvas->drawTriangle(
          cx    , cy - i, // peak
          cx - i, cy + i, // bottom left
          cx + i, cy + i, // bottom right
          ((i & 0xF8) << 8) | ((i & 0xFC) << 3) | (i >> 3));
      }
    }

    Label timeLabel;
    Adafruit_FT6206* ts;
    Adafruit_ILI9341* tft;
    RTC_PCF8523* rtc;
    Grid* grid;

  public:

    clockApp(Adafruit_FT6206* ts, Adafruit_ILI9341* tft, RTC_PCF8523* rtc, Grid* grid) 
    : ts(ts), 
      tft(tft), 
      rtc(rtc), grid(grid),timeLabel("hello world", 0, 0, 6, 6, 0x18c3, 0xFFFF, 1, true, tft, grid, 4, 4, 2, 0x0000, 10, &FreeSans12pt7b, &clockApp::drawClockBackground){}

    void appSetup() {
      
    }

    void appLoop() {
      DateTime now = rtc->now();

      int hour12 = now.hour();
      if(hour12 == 0) {
        hour12 = 12; // 12 AM
      } else if(hour12 > 12) {
        hour12 -= 12;
      }

      String hourString = (hour12 < 10) ? "0" + String(hour12) : String(hour12);
      String minuteString = (now.minute() < 10) ? "0" + String(now.minute(), DEC) : String(now.minute(), DEC);
      String timeString = hourString + ":" + minuteString;

      timeLabel.setText(timeString);
      timeLabel.draw();
    }
};


#endif
