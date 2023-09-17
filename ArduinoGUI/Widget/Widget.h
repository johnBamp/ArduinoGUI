#ifndef WIDGET_H
#define WIDGET_H

#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include "../Style.h"
#include "../ImageDrawer/ImageDrawer.h"
#include <SdFat.h>

class Widget {
  protected:
    int x, y, width, height, columnSpan, rowSpan;
    Adafruit_ILI9341* tft;
    Adafruit_FT6206* ts;
    Style style;
    String text = "";
    SdFat& sd;

  public:
    Widget(Adafruit_ILI9341* tft, Adafruit_FT6206* ts, SdFat& sdInstance);
    virtual void draw(GFXcanvas16* canvas);
      
    void setPosition(int newX, int newY);
    void setSize(int w, int h);
    void setColumnSpan(int newColumnSpan);
    void setRowSpan(int newRowSpan);
    void setStyle(Style newStyle);
    void setText (String newText);

    int getColumnSpan() const;
    int getRowSpan() const;
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    Style getStyle() const;
    String getText() const;
};

#endif // WIDGET_H
