#include "Menu.h"

Menu::Menu(Adafruit_ILI9341* tft, Adafruit_FT6206* ts, SdFat& sdInstance) : Widget(tft, ts, sdInstance) {}

void Menu::draw(GFXcanvas16* canvas) { // Add a const reference to Style
  for(int i = 0; i < widgets.size(); i++) {
    if(widgets[i] == nullptr) continue;

    int columnSpan = widgets[i]->getColumnSpan();
    int rowSpan = widgets[i]->getRowSpan();

    int newWidth = (getWidth() / columns - 2 * style.xMargin) * columnSpan; // subtracting the margins
    int newHeight = (getHeight() / rows - 2 * style.yMargin) * rowSpan;     // subtracting the margins

    int newX = widgets[i]->getX() * (getWidth() / columns) + style.xMargin; // adding the x margin
    int newY = widgets[i]->getY() * (getHeight() / rows) + style.yMargin;   // adding the y margin

    widgets[i]->setPosition(newX + getX(), newY + getY());
    widgets[i]->setSize(newWidth, newHeight);

    widgets[i]->draw(canvas);
  }
}

void Menu::setPosition(int newX, int newY) {
  x = newX;
  y = newY;
}

void Menu::setSize(int w, int h) {
  width = w;
  height = h;
}

void Menu::setColumns(int c) {
  columns = c;
}

void Menu::setRows(int r) {
  rows = r;
}

void Menu::setStyle(Style newStyle){
  style = newStyle;
  for(int i = 0; i < widgets.size(); i++) {
    if(widgets[i] == nullptr) continue;
    widgets[i]->setStyle(style);
  }
}

int Menu::getX() const {
  return x;
}

int Menu::getY() const {
  return y;
}

int Menu::getWidth() const {
  return width;
}

int Menu::getHeight() const {
  return height;
}

int Menu::getColumns() const {
  return columns;
}

int Menu::getRows() const {
  return rows;
}

Style Menu::getStyle() const{
  return style;
}

void Menu::addWidget(Widget* widget) {
  widgets.push_back(widget);
}
