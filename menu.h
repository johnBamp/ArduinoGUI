#ifndef MENU_H
#define MENU_H

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

class Menu {
  private:

    typedef void (*DrawIconFunction)(GFXcanvas16*);
    DrawIconFunction drawIconFunctions[3][3];

    Adafruit_FT6206* ts;
    Adafruit_ILI9341* tft;
    Grid* grid;

    Button* apps[3][3]; // 3x3 grid of Button pointers

    static void printHello(){
    }

  public:

    Menu(Adafruit_FT6206* ts, Adafruit_ILI9341* tft, Grid* grid) 
      : ts(ts), tft(tft), grid(grid) {

      drawIconFunctions[0][0] = &Menu::drawIcon1;
      drawIconFunctions[0][1] = &Menu::drawIcon2;
      drawIconFunctions[0][2] = &Menu::drawIcon3;
      drawIconFunctions[1][0] = &Menu::drawIcon4;
      drawIconFunctions[1][1] = &Menu::drawIcon5;
      drawIconFunctions[1][2] = &Menu::drawIcon6;
      drawIconFunctions[2][0] = &Menu::drawIcon7;
      drawIconFunctions[2][1] = &Menu::drawIcon8;
      drawIconFunctions[2][2] = &Menu::drawIcon9;

      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          apps[i][j] = new Button("",i, j, 1,1,0x18c3, 0xFFFF, 2, true, tft,grid,4, 4, 2, 0x0000, 10, &Menu::printHello, ts, ILI9341_RED, &FreeSans12pt7b, drawIconFunctions[i][j]);
        }
      }
    }

    static void drawIcon1(GFXcanvas16* canvas) {
        int centerX = canvas->width() / 2;
        int centerY = canvas->height() / 2;
        
        canvas->fillCircle(centerX, centerY, 20, ILI9341_WHITE);
        canvas->fillCircle(centerX, centerY, 2, ILI9341_BLACK);

        // Draw 1st line
        canvas->drawLine(centerX, centerY, centerX + 13, centerY + 13, ILI9341_BLACK);
        canvas->drawLine(centerX + 1, centerY, centerX + 14, centerY + 13, ILI9341_BLACK);
        canvas->drawLine(centerX, centerY + 1, centerX + 13, centerY + 14, ILI9341_BLACK);
        
        // Draw 2nd line
        canvas->drawLine(centerX, centerY, centerX - 8, centerY + 8, ILI9341_BLACK);
        canvas->drawLine(centerX + 1, centerY, centerX - 7, centerY + 8, ILI9341_BLACK);
        canvas->drawLine(centerX, centerY + 1, centerX - 8, centerY + 9, ILI9341_BLACK);
    }
    static void drawIcon2(GFXcanvas16* canvas) { /* ... */ }
    static void drawIcon3(GFXcanvas16* canvas) { /* ... */ }
    static void drawIcon4(GFXcanvas16* canvas) { /* ... */ }
    static void drawIcon5(GFXcanvas16* canvas) { /* ... */ }
    static void drawIcon6(GFXcanvas16* canvas) { /* ... */ }
    static void drawIcon7(GFXcanvas16* canvas) { /* ... */ }
    static void drawIcon8(GFXcanvas16* canvas) { /* ... */ }
    static void drawIcon9(GFXcanvas16* canvas) { /* ... */ }
    static void drawIcon10(GFXcanvas16* canvas) { /* ... */ }
    static void drawIcon11(GFXcanvas16* canvas) { /* ... */ }
    static void drawIcon12(GFXcanvas16* canvas) { /* ... */ }

    void appSetup() {
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          apps[i][j]->setBackgroundColor(0x18c3);
          apps[i][j]->draw();
        } 
      }
      delay(1000);
    }

    int appLoop() {
        int l = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if(apps[i][j] != nullptr){
                    apps[i][j]->checkTouch();
                    if(apps[i][j]->isPressed){
                        if(l == 1){
                            return 3;
                        }
                        return l;
                    }
                    l++;
                }
            }
        }
        return 2;
    }

    ~Menu() {  // Destructor to delete allocated memory
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          if(apps[i][j] != nullptr) {
            delete apps[i][j];
            apps[i][j] = nullptr;
          }
        }
      }
    }
};


#endif
