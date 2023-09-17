#ifndef MENU_H
#define MENU_H

#include "../Widget/Widget.h"
#include <vector>

class Menu : Widget {
  private:
    int columns, rows;
    std::vector<Widget*> widgets;

  public:
    Menu(Adafruit_ILI9341* tft, Adafruit_FT6206* ts, SdFat& sdInstance);
    void draw(GFXcanvas16* canvas) override;

    void setPosition(int newX, int newY);
    void setSize(int w, int h);
    void setColumns(int c);
    void setRows(int r);
    void setStyle(Style newStyle);

    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    int getColumns() const;
    int getRows() const;
    Style getStyle() const;

    void addWidget(Widget* widget);
    
};

#endif // MENU_H
