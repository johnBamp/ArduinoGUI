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
#include "clockApp.h"
#include "wifiApp.h"

RTC_PCF8523 rtc;

#define TFT_CS 14
#define TFT_DC 32
#define SD_CS 15
#define baud_rate 115200

Adafruit_FT6206 ts = Adafruit_FT6206();
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

std::vector<Button> apps;

int appsPerRow = 4;
int appsPerColumn = 3;
int appWidth;
int appHeight;

int margin = 10; // Space between the sides and top/bottom
int spacing = 10; // Space between each app


ClockApp clockScreen(&tft, &ts, &rtc);
WifiApp wifiScreen(&tft, &ts);

std::vector<BaseApp*> appScreens;

void setup() {
  Serial.begin(115200);

  while(!Serial);

  tft.begin();
  ts.begin();

  tft.setRotation(1);

  tft.fillScreen(ILI9341_BLACK);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  rtc.start();


  appWidth = (tft.width() - margin * 2 - spacing * (appsPerRow - 1)) / appsPerRow;
  appHeight = (tft.height() - margin * 2 - spacing * (appsPerColumn - 1)) / appsPerColumn;

  appScreens.push_back(&clockScreen);  // Add your app screens here
  appScreens.push_back(&wifiScreen);

  for (int i = 0; i < appScreens.size(); i++) {
    int x = margin + (i % appsPerRow) * (appWidth + spacing); // Adjust x to account for margin and spacing
    int y = margin + (i / appsPerRow) * (appHeight + spacing); // Adjust y to account for margin and spacing

    Button app(x, y, appWidth, appHeight, ILI9341_BLACK, &ts, &tft);
    app.setDimensions(appWidth, appHeight, 40, 1);
    app.setColor(0x2104, 0x2104, ILI9341_WHITE);

    //app.setText("App " + String(i + 1), 1, true);
    app.setFunction([i](GFXcanvas16& canvas) { appScreens[i]->icon(canvas); });


    app.addInverseColor(ILI9341_WHITE);
    apps.push_back(app);
  }
}

int screenValue = 2;
int oldScreenValue = 3;

int startX = -1;
int startY = -1;

int prevX = 0;
int prevY = 0;

bool isDragging = false;

void detectDrag() {
  TS_Point p;
  if(ts.touched()){
    p = ts.getPoint();

    // Correct the touch orientation for your screen setup
    int swap = p.y;
    p.y = p.x;
    p.x = swap;
    p.x = map(p.x, 320, 0, 0, 320);

    // Start drag detection
    if (!isDragging) {
      startX = p.x;
      startY = p.y;
      isDragging = true;
    }
  } else {
    if (isDragging) {
      isDragging = false;
            
      // Check if it was a valid down drag when the touch ends
      if ((prevY - startY) >= 80) {
        // Detected valid downward drag
        screenValue = 0;
      }

      // Reset starting positions
      startX = -1;
      startY = -1;
    }
  }

  prevX = p.x;
  prevY = p.y;

  if(screenValue == 0){
    for (int i = 0; i < apps.size(); i++) {
      if (apps[i].checkTouched()) {
        screenValue = i + 2;
      }
    }
  }
}

void loop() {

  if (screenValue == 0) {
    for (int i = 0; i < apps.size(); i++) {
      if (apps[i].checkTouched()) {
        screenValue = i + 2;  // Just increment by 1 now since you start from 0
      }
    }
  }

  detectDrag();

  if(screenValue != oldScreenValue){
    tft.fillScreen(ILI9341_BLACK);
    if(screenValue == 0){
      for (int i = 0; i < apps.size(); i++) {
        apps[i].draw();
      }
    }else{
      appScreens[screenValue - 2]->appSetup();
    }
  }

  if(screenValue != 0){
    appScreens[screenValue - 2]->appLoop();
  }

  oldScreenValue = screenValue;
}
