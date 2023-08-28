#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

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

class Notification {
  private:
    Label notifTimeLabel;
    Label batteryLabel;
    Label dateLabel;
    Label NotificationLabel1;
    Label NotificationLabel2;
    Label NotificationLabel3;
    Label NotificationLabel4;

    Adafruit_FT6206* ts;
    Adafruit_ILI9341* tft;
    Grid* grid;
    RTC_PCF8523* rtc;

    void drawScreen(){
      notifTimeLabel.draw();
      batteryLabel.draw();
      dateLabel.draw();

      NotificationLabel1.draw();
      NotificationLabel2.draw();
      NotificationLabel3.draw();
      NotificationLabel4.draw();
    }

  public:
    Notification(Adafruit_FT6206* ts, Adafruit_ILI9341* tft, RTC_PCF8523* rtc, Grid* grid) 
      : ts(ts), 
        tft(tft), 
        rtc(rtc),
        grid(grid),
        notifTimeLabel("0:00", 1, 0, 1, 4, 0x18c3, 0xFFFF, 1, true, tft, grid, 4, 4, 2, 0x0000, 10, &FreeSans12pt7b),
        batteryLabel("78%", 1, 4, 1, 2, 0x18c3, 0xFFFF, 1, true, tft, grid, 4, 4, 2, 0x0000, 10, &FreeSans12pt7b),
        dateLabel("Sunday, August 27", 0, 0, 1, 6, 0x18c3, 0xFFFF, 1, true, tft, grid, 4, 4, 2, 0x0000, 10, &FreeSans12pt7b),
        NotificationLabel1("", 2, 0, 1, 6, 0x18c3, 0xFFFF, 1, true, tft, grid, 4, 4, 2, 0x0000, 10, &FreeSans12pt7b),
        NotificationLabel2("", 3, 0, 1, 6, 0x18c3, 0xFFFF, 1, true, tft, grid, 4, 4, 2, 0x0000, 10, &FreeSans12pt7b),
        NotificationLabel3("", 4, 0, 1, 6, 0x18c3, 0xFFFF, 1, true, tft, grid, 4, 4, 2, 0x0000, 10, &FreeSans12pt7b),
        NotificationLabel4("", 5, 0, 1, 6, 0x18c3, 0xFFFF, 1, true, tft, grid, 4, 4, 2, 0x0000, 10, &FreeSans12pt7b)
    {}

    void appSetup() {
      for(int i = -240; i <= 0; i += 20){
        grid->setY(i);
        drawScreen();
      }
    }

    String dayOfWeek(int day) {
        switch(day) {
            case 0: return "Sunday";
            case 1: return "Monday";
            case 2: return "Tuesday";
            case 3: return "Wednesday";
            case 4: return "Thursday";
            case 5: return "Friday";
            case 6: return "Saturday";
            default: return "";
        }
    }

    String monthName(int month) {
        switch(month) {
            case 1: return "January";
            case 2: return "February";
            case 3: return "March";
            case 4: return "April";
            case 5: return "May";
            case 6: return "June";
            case 7: return "July";
            case 8: return "August";
            case 9: return "September";
            case 10: return "October";
            case 11: return "November";
            case 12: return "December";
            default: return "";
        }
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
        notifTimeLabel.setText(timeString);
        notifTimeLabel.draw();

        String dateString = dayOfWeek(now.dayOfTheWeek()) + ", " + monthName(now.month()) + " " + String(now.day());
        dateLabel.setText(dateString);
        dateLabel.draw();

        int batteryRaw = analogRead(A13);
        int batteryPercentage = map(batteryRaw, 0, 4095, 0, 100);
        batteryPercentage = constrain(batteryPercentage, 0, 100);  // Ensure percentage is between 0% and 100%
        batteryLabel.setText(String(batteryPercentage) + "%");
        batteryLabel.draw();
    }

};



#endif
