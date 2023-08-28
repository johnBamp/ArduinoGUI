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
#include <Fonts/FreeSans9pt7b.h>
#include "clockApp.h"
#include "notifcations.h"
#include "menu.h"

RTC_PCF8523 rtc;

#define TFT_CHIP_SLCT 13
#define TFT_DATA_CMD 12

#define baud_rate 115200
    
Adafruit_FT6206 ts = Adafruit_FT6206();
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CHIP_SLCT, TFT_DATA_CMD);

Grid timeGrid = Grid(6, 6, &tft, 320, 240, 0, 0);
Grid notificationGrid = Grid(6, 6, &tft, 320, 240, 0, 0);
Grid menuGrid = Grid(3, 3, &tft, 320, 240, 0, 0);

clockApp timeApp(&ts, &tft, &rtc, &timeGrid);
Notification notifications(&ts, &tft, &rtc, &notificationGrid);
Menu menu(&ts, &tft, &menuGrid);

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {
  Serial.begin(baud_rate);
  while (!Serial);
  tft.begin();
  ts.begin();
  tft.setRotation(3);
  tft.fillScreen(0x0000);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.start();
  float drift = 43; // seconds plus or minus over oservation period - set to 0 to cancel previous calibration.
  float period_sec = (7 * 86400);  // total obsevation period in seconds (86400 = seconds in 1 day:  7 days = (7 * 86400) seconds )
  float deviation_ppm = (drift / period_sec * 1000000); //  deviation in parts per million (μs)
  float drift_unit = 4.34; // use with offset mode PCF8523_TwoHours
  int offset = round(deviation_ppm / drift_unit);
  Serial.print("Offset is "); Serial.println(offset); // Print to control offset
}

int appState = 0;
int oldAppState = 0;
int oldAppState2 = 0;

TS_Point startPoint;
TS_Point endPoint;
bool dragging = false;



void loop() {


  if(ts.touched()){
      TS_Point p = ts.getPoint();
      
      // If dragging is not yet started, save the initial touch point
      if(!dragging){
        startPoint = p;
        dragging = true;
      }

      endPoint = p;  // Continuously update the end point while dragging

      // Check if the user dragged from top to bottom
      if(dragging && map(endPoint.x, 0, 240, 240, 0) - map(startPoint.x, 0, 240, 240, 0) >= 160){
        if(appState != 1){  // Only save the old state if we're not already in the notification state
            oldAppState2 = appState;
        }
        appState = 1;
        dragging = false; // Reset dragging for next touch
      }

      // Check if the user dragged from bottom to top
      if(dragging && map(startPoint.x, 0, 240, 240, 0) - map(endPoint.x, 0, 240, 240, 0) >= 160){
        if(appState == 1){
          appState = oldAppState2;
        } else {
          appState = 2;
        }
        dragging = false; // Reset dragging for next touch
      }
  } else {
      dragging = false;  // Reset dragging if screen is not touched
  }


  if(appState != oldAppState){
    tft.fillScreen(ILI9341_BLACK);
    if(appState == 0){
      timeApp.appSetup();
    }else if(appState == 1){
      notifications.appSetup();
    }else if(appState == 2){
      menu.appSetup();
    }

  }

  oldAppState = appState;

  if(appState == 0){
    timeApp.appLoop();
  }else if(appState == 1){
    notifications.appLoop();
  }else if(appState == 2){
    appState = menu.appLoop();
  }
  
}
