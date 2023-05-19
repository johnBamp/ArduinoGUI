#ifndef ARDUINOGUI_H
#define ARDUINOGUI_H

#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <Adafruit_FT6206.h>

#define TFT_CHIP_SLCT 32
#define TFT_DATA_CMD 33

#define baud_rate 9600

class Grid {
  private:
    int rows;
    int columns;
    Adafruit_ILI9341* tft;

  public:
    Grid(int rows, int columns, Adafruit_ILI9341* tft);
    int getRows();
    int getColumns();
    void setRows(int r);
    void setColumns(int c);
};

class Label {
  protected:
    String text;
    int row;
    int column;
    int rowspan;
    int colspan;
    uint16_t backgroundColor;
    uint16_t textColor;
    uint8_t textSize;
    bool centered;
    Adafruit_ILI9341* tft;
    Grid* grid;
    int padx;  // Padding x
    int pady;  // Padding y
    int border;  // Border width
    uint16_t borderColor;  // Border color
    int radius;  // Radius for rounded corners
    bool hidden;
    int topLine;

  public:
    Label(String text, int row, int column, int rowspan, int colspan, uint16_t backgroundColor, uint16_t textColor, uint8_t textSize, bool centered, Adafruit_ILI9341* tft, Grid* grid, int padx, int pady, int border, uint16_t borderColor, int radius);
    void draw();
    void hide();
    void setTopLine(int line);
    int getTopLine();
    void setText(String t);
    String getText();
    void setRow(int r);
    int getRow();
    void setColumn(int c);
    int getColumn();
    void setRowspan(int rs);
    int getRowspan();
    void setColspan(int cs);
    int getColspan();
    void setBackgroundColor(uint16_t color);
    uint16_t getBackgroundColor();
    bool isHidden();
};

class ScrollableLabel : public Label {
  private:
    Adafruit_FT6206* cts;

  public:
    ScrollableLabel(String text, int row, int column, int rowspan, int colspan, uint16_t backgroundColor, uint16_t textColor, uint8_t textSize, bool centered, Adafruit_ILI9341* tft, Grid* grid, int padx, int pady, int border, uint16_t borderColor, int radius, Adafruit_FT6206* cts, int topLine = 0);
    void checkTouch();
};

class Button : public Label {
  private:
    void (*onClick)();
    Adafruit_FT6206* cts;
    uint16_t activeColor;
    uint16_t inactiveColor;
    bool isPressed;
    int radius;

  public:
    Button(String text, int row, int column, int rowspan, int colspan, uint16_t backgroundColor, uint16_t textColor, uint8_t textSize, bool centered, Adafruit_ILI9341* tft, Grid* grid, int padx, int pady, int border, uint16_t borderColor, int radius, void (*onClick)(), Adafruit_FT6206* cts, uint16_t activeColor);
    void checkTouch();
};

#endif
