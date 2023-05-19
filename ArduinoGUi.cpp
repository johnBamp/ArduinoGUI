#include "arduinoGUI.h"

Grid::Grid(int rows, int columns, Adafruit_ILI9341* tft): rows(rows), columns(columns), tft(tft) {}

int Grid::getRows() {
  return rows;
}

int Grid::getColumns() {
  return columns;
}

void Grid::setRows(int r) {
  rows = r;
}

void Grid::setColumns(int c) {
  columns = c;
}

Label::Label(String text, int row, int column, int rowspan, int colspan, uint16_t backgroundColor, uint16_t textColor, uint8_t textSize, bool centered, Adafruit_ILI9341* tft, Grid* grid, int padx, int pady, int border, uint16_t borderColor, int radius)
  : text(text), row(row), column(column), rowspan(rowspan), colspan(colspan), backgroundColor(backgroundColor), textColor(textColor), textSize(textSize), centered(centered), tft(tft), grid(grid), padx(padx), pady(pady), border(border), borderColor(borderColor), radius(radius) {}

void Label::draw() {
  hidden = false;

  int x = (tft->width() / grid->getColumns()) * column;
  int y = (tft->height() / grid->getRows()) * row;
  int w = (tft->width() / grid->getColumns()) * colspan;
  int h = (tft->height() / grid->getRows()) * rowspan;

  // Draw the border
  tft->fillRoundRect(x, y, w, h, radius, borderColor);
  // Draw the background color inside the border
  tft->fillRoundRect(x + border, y + border, w - 2 * border, h - 2 * border, radius, backgroundColor);

  tft->setTextColor(textColor);
  tft->setTextSize(textSize);

  // Adjust the drawing area for padding
  x += padx + border;
  y += pady + border;
  w -= 2 * (padx + border);
  h -= 2 * (pady + border);

  // Estimate max characters per line
  int maxCharsPerLine = w / (6 * textSize);

  // Calculate the maximum lines that can fit in the box
  int maxLines = h / (8 * textSize);

  // Simple text wrapping and line counting
  String line = "";
  String word = "";
  int lineCount = 0;
  for (int i = 0; i < text.length() && lineCount < maxLines + topLine; i++) {
    char c = text[i];
    if (c == ' ' || c == '\n' || i == text.length() - 1) {
      if (i == text.length() - 1 && c != ' ' && c != '\n') {
        word += c;
      }
      if (line.length() + word.length() <= maxCharsPerLine) {
        line += word;
        if (c == ' ') {
          line += " ";
        }
      } else {
        if (lineCount >= topLine) {
          int newX = centered ? x + (w - line.length() * 6 * textSize) / 2 : x;
          tft->setCursor(newX, y);
          tft->println(line);
          y += 8 * textSize;
        }
        line = word + " ";
        lineCount++;
      }
      word = "";
    } else {
      word += c;
    }
  }

  // Print the last line if it's not empty and doesn't exceed maxLines
  if (line.length() > 0 && lineCount >= topLine && lineCount < maxLines + topLine) {
    int newX = centered ? x + (w - line.length() * 6 * textSize) / 2 : x;
    tft->setCursor(newX, y);
    tft->println(line);
  }
}

void Label::hide() {
  int x = (tft->width() / grid->getColumns()) * column;
  int y = (tft->height() / grid->getRows()) * row;
  int w = (tft->width() / grid->getColumns()) * colspan;
  int h = (tft->height() / grid->getRows()) * rowspan;

  hidden = true;

  // Draw over the area with the background color
  tft->fillRoundRect(x, y, w, h, radius, ILI9341_BLACK);
}

void Label::setTopLine(int line) {
  topLine = line;
}

int Label::getTopLine() {
  return topLine;
}

void Label::setText(String t) {
  text = t;
}

String Label::getText() {
  return text;
}

void Label::setRow(int r) {
  row = r;
}

int Label::getRow() {
  return row;
}

void Label::setColumn(int c) {
  column = c;
}

int Label::getColumn() {
  return column;
}

void Label::setRowspan(int rs) {
  rowspan = rs;
}

int Label::getRowspan() {
  return rowspan;
}

void Label::setColspan(int cs) {
  colspan = cs;
}

int Label::getColspan() {
  return colspan;
}

void Label::setBackgroundColor(uint16_t color) {
  backgroundColor = color;
}

uint16_t Label::getBackgroundColor() {
  return backgroundColor;
}

bool Label::isHidden() {
  return hidden;
}

ScrollableLabel::ScrollableLabel(String text, int row, int column, int rowspan, int colspan, uint16_t backgroundColor, uint16_t textColor, uint8_t textSize, bool centered, Adafruit_ILI9341* tft, Grid* grid, int padx, int pady, int border, uint16_t borderColor, int radius, Adafruit_FT6206* cts, int topLine)
  : Label(text, row, column, rowspan, colspan, backgroundColor, textColor, textSize, centered, tft, grid, padx, pady, border, borderColor, radius), cts(cts) {}

void ScrollableLabel::checkTouch() {
  if (cts->touched()) {
    TS_Point touchPoint = cts->getPoint();
    int x = (tft->width() / grid->getColumns()) * column;
    int y = (tft->height() / grid->getRows()) * row;
    int w = (tft->width() / grid->getColumns()) * colspan;
    int h = (tft->height() / grid->getRows()) * rowspan;

    int swap = touchPoint.x;
    touchPoint.x = touchPoint.y;
    touchPoint.y = swap;

    touchPoint.x = map(touchPoint.x, 320, 0, 0, 320);

    if (touchPoint.x >= x && touchPoint.x <= x + w && touchPoint.y >= y && touchPoint.y <= y + h) {
      // Estimate max characters per line
      int maxCharsPerLine = w / (6 * textSize);

      // Calculate the maximum lines that can fit in the box
      int maxLines = h / (8 * textSize);

      // Count lines in text
      int lineCount = 0;
      for (int i = 0; i < text.length(); i++) {
        if (text[i] == ' ' || text[i == '\n') {
          lineCount++;
        }
      }

      if (touchPoint.y > (y + h / 2) && topLine < lineCount - maxLines - 18) {
        topLine++;
        draw();
      }
      if (touchPoint.y < (y + h / 2) && topLine != 0) {
        topLine--;
        draw();
      }
    }
  }
}

Button::Button(String text, int row, int column, int rowspan, int colspan, uint16_t backgroundColor, uint16_t textColor, uint8_t textSize, bool centered, Adafruit_ILI9341* tft, Grid* grid, int padx, int pady, int border, uint16_t borderColor, int radius, void (*onClick)(), Adafruit_FT6206* cts, uint16_t activeColor)
  : Label(text, row, column, rowspan, colspan, backgroundColor, textColor, textSize, centered, tft, grid, padx, pady, border, borderColor, radius),
    onClick(onClick), cts(cts), activeColor(activeColor), inactiveColor(backgroundColor), isPressed(false), radius(radius) {}

void Button::checkTouch() {
  if (cts->touched()) {
    TS_Point touchPoint = cts->getPoint();
    int x = (tft->width() / grid->getColumns()) * column;
    int y = (tft->height() / grid->getRows()) * row;
    int w = (tft->width() / grid->getColumns()) * colspan;
    int h = (tft->height() / grid->getRows()) * rowspan;

    int swap = touchPoint.x;
    touchPoint.x = touchPoint.y;
    touchPoint.y = swap;

    touchPoint.x = map(touchPoint.x, 320, 0, 0, 320);

    if (touchPoint.x >= x && touchPoint.x <= x + w && touchPoint.y >= y && touchPoint.y <= y + h) {
      if (!isPressed) {
        // Only change the color and call the onClick function if the button was not already pressed
        setBackgroundColor(activeColor);
        draw();
        onClick();
        isPressed = true;
      }
    } else {
      if (isPressed) {
        // If the touch is outside the button area and the button was previously pressed, change its color back to the inactive color
        setBackgroundColor(inactiveColor);
        draw();
        isPressed = false;
      }
    }
  } else {
    if (isPressed) {
      // If the button was previously pressed, change its color back to the inactive color when the touch is released
      setBackgroundColor(inactiveColor);
      draw();
      isPressed = false;
    }
  }
}
