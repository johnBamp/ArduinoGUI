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
    Label(String text, int row, int column, int rowspan, int colspan, uint16_t backgroundColor, uint16_t textColor, uint8_t textSize, bool centered, Adafruit_ILI9341* tft, Grid* grid, int padx, int pady, int border, uint16_t borderColor, int radius):
      text(text), row(row), column(column), rowspan(rowspan), colspan(colspan), backgroundColor(backgroundColor), textColor(textColor), textSize(textSize), centered(centered), tft(tft), grid(grid), padx(padx), pady(pady), border(border), borderColor(borderColor), radius(radius) {}

    void draw() {

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

    void hide() {
      int x = (tft->width() / grid->getColumns()) * column;
      int y = (tft->height() / grid->getRows()) * row;
      int w = (tft->width() / grid->getColumns()) * colspan;
      int h = (tft->height() / grid->getRows()) * rowspan;

      hidden = true;

      // Draw over the area with the background color
      tft->fillRoundRect(x, y, w, h, radius, ILI9341_BLACK);
    }

    void setTopLine(int line) {
      topLine = line;
    }
    int getTopLine() {
      return topLine;
    }

    void setText(String t) {
      text = t;
    }
    String getText() {
      return text;
    }

    void setRow(int r) {
      row = r;
    }
    int getRow() {
      return row;
    }

    void setColumn(int c) {
      column = c;
    }
    int getColumn() {
      return column;
    }

    void setRowspan(int rs) {
      rowspan = rs;
    }
    int getRowspan() {
      return rowspan;
    }

    void setColspan(int cs) {
      colspan = cs;
    }
    int getColspan() {
      return colspan;
    }

    void setBackgroundColor(uint16_t color) {
      backgroundColor = color;
    }
    uint16_t getBackgroundColor() {
      return backgroundColor;
    }

    bool isHidden() {
      return hidden;
    }
};
