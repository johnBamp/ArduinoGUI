#ifndef ARDUINOGUI_H
#define ARDUINOGUI_H

#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <Adafruit_FT6206.h>
#include <Adafruit_ImageReader.h> // Image-reading functions
#include <SdFat.h>
#include <SPIFFS.h>

#define TFT_CHIP_SLCT 32
#define TFT_DATA_CMD 33

#define baud_rate 9600

class Grid {
  private:
    int rows;
    int columns;

    int width;  // width of the grid
    int height; // height of the grid
    
    int gridX;  // x-coordinate of the grid
    int gridY;  // y-coordinate of the grid
    
    Adafruit_ILI9341* tft;
  

  public:
    Grid(int rows, int columns, Adafruit_ILI9341* tft, int width, int height, int x = 0, int y = 0) : rows(rows), columns(columns), tft(tft), width(width), height(height), gridX(x), gridY(y) {}

    int getRows() {
      return rows;
    }

    int getColumns() {
      return columns;
    }

    int getWidth() {
      return width;
    }

    int getHeight() {
      return height;
    }

    int getX() {
      return gridX;
    }

    int getY() {
      return gridY;
    }

    void setRows(int r) {
      rows = r;
    }

    void setColumns(int c) {
      columns = c;
    }

    void setX(int x) {
      gridX = x;
    }

    void setY(int y) {
      gridY = y;
    }


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
    int padx; 
    int pady; 
    int border;
    uint16_t borderColor; 
    int radius; 
    bool hidden;
    int topLine = 0;
    const GFXfont* customFont;
    String image = ""; // New member variable to store the image
    void (*functionPtr)(GFXcanvas16*);
    int x = -1; // Default to -1 to indicate they are not set
    int y = -1;

  public:
    Label(String text, int row, int column, int rowspan, int colspan, uint16_t backgroundColor, uint16_t textColor, uint8_t textSize, bool centered, Adafruit_ILI9341* tft, Grid* grid, int padx, int pady, int border, uint16_t borderColor, int radius, const GFXfont* font = NULL, void (*functionPtr)(GFXcanvas16*) = nullptr)
        : text(text), row(row), column(column), rowspan(rowspan), colspan(colspan), backgroundColor(backgroundColor), textColor(textColor), textSize(textSize), centered(centered), tft(tft), grid(grid), padx(padx), pady(pady), border(border), borderColor(borderColor), radius(radius), customFont(font), functionPtr(functionPtr) {}

    void draw() {
        hidden = false;

        int drawX = x != -1 ? x : (grid->getX() + (grid->getWidth() / grid->getColumns()) * column);
        int drawY = y != -1 ? y : (grid->getY() + (grid->getHeight() / grid->getRows()) * row);
        int w = (grid->getWidth() / grid->getColumns()) * colspan;
        int h = (grid->getHeight() / grid->getRows()) * rowspan;

        int32_t width, height;

        int16_t x1, y1;
        uint16_t w1, h1;

        GFXcanvas16 canvas(w, h);
        
        if (customFont) {
          canvas.setFont(customFont);
        } else {
          canvas.setFont(NULL);
        }

        canvas.setTextSize(textSize);
        canvas.getTextBounds(text, 0, 0, &x1, &y1, &w1, &h1);

        int canvasX = padx + border;
        int canvasY = pady + border;
        int canvasW = w - 2 * (padx + border);
        int canvasH = h - 2 * (pady + border);

        if (centered) {
          canvasX += (canvasW - w1) / 2;
          canvasY += (canvasH - h1) / 2 - y1;
        } else {
          canvasY = 24;
        }

        canvas.fillRoundRect(0, 0, w, h, radius, borderColor);
        canvas.fillRoundRect(border, border, w - 2 * border, h - 2 * border, radius, backgroundColor);

        if (functionPtr != nullptr) {
          (*functionPtr)(&canvas); // If canvas is the GFXcanvas16* you want to pass to the function
        }

        canvas.setTextColor(textColor);
        canvas.setCursor(canvasX, canvasY);
        canvas.print(text);

        tft->drawRGBBitmap(drawX, drawY, canvas.getBuffer(), w, h);    
    }

    void hide() {
      int drawX = x != -1 ? x : (tft->width() / grid->getColumns()) * column;
      int drawY = y != -1 ? y : (tft->height() / grid->getRows()) * row;
      int w = (tft->width() / grid->getColumns()) * colspan;
      int h = (tft->height() / grid->getRows()) * rowspan;

      hidden = true;

      

      // Draw over the area with the background color
      tft->fillRoundRect(drawX, drawY, w, h, radius, ILI9341_BLACK);
    }

    void setTopLine(int line) {
      topLine = line;
    }

    int getTopLine() {
      return topLine;
    }

    void setText(String t) {
      text = t;
      draw();
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

    void setPos(int xPos, int yPos) {
      x = xPos;
      y = yPos;
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    int getHeight() {
      // Compute the height based on grid values if y is not set
      if(y == -1) {
        return (grid->getHeight() / grid->getRows()) * rowspan; // Adjusted to use grid height
      }
      return y; // If y is set, it denotes a custom height
    }
        
    int getWidth() {
      // Compute the width based on grid values if x is not set
      if(x == -1) {
        return (grid->getWidth() / grid->getColumns()) * colspan; // Adjusted to use grid width
      }
      return x; // If x is set, it denotes a custom width
    }


    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

};

class ScrollableLabel : public Label {
  private:
    Adafruit_FT6206* cts;

  public:
    ScrollableLabel(String text, int row, int column, int rowspan, int colspan, uint16_t backgroundColor, uint16_t textColor, uint8_t textSize, bool centered, Adafruit_ILI9341* tft, Grid* grid, int padx, int pady, int border, uint16_t borderColor, int radius, Adafruit_FT6206* cts, int topLine)
      : Label(text, row, column, rowspan, colspan, backgroundColor, textColor, textSize, centered, tft, grid, padx, pady, border, borderColor, radius), cts(cts) {}

    void checkTouch() {
      if (cts->touched()) {
        TS_Point touchPoint = cts->getPoint();
        int x = (tft->width() / grid->getColumns()) * column;
        int y = (tft->height() / grid->getRows()) * row;
        int w = (tft->width() / grid->getColumns()) * colspan;
        int h = (tft->height() / grid->getRows()) * rowspan;

        int swap = touchPoint.x;
        touchPoint.x = touchPoint.y;
        touchPoint.y = swap;

        touchPoint.y = map(touchPoint.y, 240, 0, 0, 240);

        if (touchPoint.x >= x && touchPoint.x <= x + w && touchPoint.y >= y && touchPoint.y <= y + h) {
          // Estimate max characters per line
          int maxCharsPerLine = w / (6 * textSize);

          // Calculate the maximum lines that can fit in the box
          int maxLines = h / (8 * textSize);

          // Count lines in text
          int lineCount = 0;
          for (int i = 0; i < text.length(); i++) {
            char c = text[i];
            if (c == ' ' || c == '\n') {
              lineCount++;
            }
          }

          if (touchPoint.y > (y + h / 2)) {
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
};

class Button : public Label {
  private:
    void (*onClick)();
    Adafruit_FT6206* cts;
    uint16_t activeColor;
    uint16_t inactiveColor;
    int radius;

  public:
    bool wasPressed = false;
    bool isPressed;
    bool buttonJustPressed = false;


    static void placeholder() {

    }

    Button(String text, int row, int column, int rowspan, int colspan, uint16_t backgroundColor, uint16_t textColor, uint8_t textSize, bool centered, Adafruit_ILI9341* tft, Grid* grid, int padx, int pady, int border, uint16_t borderColor, int radius, void (*onClick)(), Adafruit_FT6206* cts, uint16_t activeColor)
      : Label(text, row, column, rowspan, colspan, backgroundColor, textColor, textSize, centered, tft, grid, padx, pady, border, borderColor, radius),
        onClick(onClick), cts(cts), activeColor(activeColor), inactiveColor(backgroundColor), isPressed(false), radius(radius) {}


    void checkTouch() {
      if (cts->touched()) {
        TS_Point touchPoint = cts->getPoint();
        int x = (tft->width() / grid->getColumns()) * column;
        int y = (tft->height() / grid->getRows()) * row;
        int w = (tft->width() / grid->getColumns()) * colspan;
        int h = (tft->height() / grid->getRows()) * rowspan;

        int swap = touchPoint.x;
        touchPoint.x = touchPoint.y;
        touchPoint.y = swap;

        touchPoint.y = map(touchPoint.y, 240, 0, 0, 240);

        if (touchPoint.x >= x && touchPoint.x <= x + w && touchPoint.y >= y && touchPoint.y <= y + h) {
          if (!isPressed && !buttonJustPressed && !wasPressed) {
            setBackgroundColor(activeColor);
            draw();
            //onClick();
            isPressed = true;
            buttonJustPressed = true;
            wasPressed = true;
          }
        } else {
          if (isPressed) {
            setBackgroundColor(inactiveColor);
            draw();
          }
          isPressed = false;
          buttonJustPressed = false;
          wasPressed = false;
        }
      } else {
        if (isPressed) {
          setBackgroundColor(inactiveColor);
          draw();
        }
        isPressed = false;
        buttonJustPressed = false;
        wasPressed = false;
      }
    }



};

class Keyboard {
    private:
      Button* keys[5][10];
      Button* modeKey;
      Button* deleteKey;
      Button* spaceKey;
      Button* enterKey;
      Adafruit_ILI9341* tft;
      Grid* grid;
      Adafruit_FT6206* cts;
      bool mode;
      int lineSkip;
      bool upperCase = true;
      String numericSpecial[5][10] = {{"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"},
        {"!", "@", "#", "$", "%", "^", "&", "*", "(", ")"},
        {"_", "+", "{", "}", "|", "~", "<", ">", "?", ":"},
        {"MODE", " ", "DEL", "ENT"}
      };

      String qwerty[5][10] = {{"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"},
        {"A", "S", "D", "F", "G", "H", "J", "K", "L", "-"},
        {"Z", "X", "C", "V", "B", "N", "M", ",", ".", "/"},
        {"MODE", " ", "DEL", "ENT"}
      };

    public:
      static void placeholder() {

      }

      Keyboard(int skip, Adafruit_ILI9341* tft, Grid* grid, Adafruit_FT6206* cts)
        : lineSkip(skip), tft(tft), grid(grid), cts(cts) {

        input = "";
        // Initialize QWERTY keys
        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 10; j++) {
            keys[i][j] = new Button(qwerty[i][j], i + 1 + lineSkip, j, 1, 1, ILI9341_WHITE, ILI9341_BLACK, 2, true, tft, grid, 2, 2, 1, ILI9341_BLACK, 5, &placeholder, cts, ILI9341_BLUE);
          }
        }

        // Initialize bottom row keys
        modeKey = new Button("mode", 4 + lineSkip, 0, 1, 2, ILI9341_WHITE, ILI9341_BLACK, 2, true, tft, grid, 2, 2, 1, ILI9341_BLACK, 5, &placeholder, cts, ILI9341_BLUE);
        spaceKey = new Button(" ", 4 + lineSkip, 2, 1, 4, ILI9341_WHITE, ILI9341_BLACK, 2, true, tft, grid, 2, 2, 1, ILI9341_BLACK, 5, &placeholder, cts, ILI9341_BLUE);
        deleteKey = new Button("Del", 4 + lineSkip, 6, 1, 2, ILI9341_WHITE, ILI9341_BLACK, 2, true, tft, grid, 2, 2, 1, ILI9341_BLACK, 5, &placeholder, cts, ILI9341_BLUE);
        enterKey = new Button("Enter", 4 + lineSkip, 8, 1, 2, ILI9341_WHITE, ILI9341_BLACK, 2, true, tft, grid, 2, 2, 1, ILI9341_BLACK, 5, &placeholder, cts, ILI9341_BLUE);
      }

      String input; // Store user input

      void toggleMode() {
        // Toggle mode between alphanumeric and numeric/special
        mode = !mode;

        // Update buttons
        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 10; j++) {
            keys[i][j]->setText(mode ? numericSpecial[i][j] : qwerty[i][j]);
            keys[i][j]->draw(); // Update the displayed button
          }
        }
        // Redraw the mode button to reflect the current mode
        modeKey->setText(mode ? "ALPHA" : "MODE");
        modeKey->draw();
      }


      void draw() {
        // Draw QWERTY keys
        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 10; j++) {
            keys[i][j]->draw();
          }
        }

        // Draw bottom row keys
        modeKey->draw();
        spaceKey->draw();
        deleteKey->draw();
        enterKey->draw();
      }

      bool wasPressed[3][10] = {false};
      bool wasModeKeyPressed = false;
      bool wasSpaceKeyPressed = false;
      bool wasDeleteKeyPressed = false;
      bool wasEnterKeyPressed = false;

      void readKeys() {
        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 10; j++) {
            keys[i][j]->checkTouch();

            if (keys[i][j]->getText() == "/" && keys[i][j]->isPressed && !wasPressed[i][j]) {
              upperCase = !upperCase;
              // Redraw the keyboard with the new case.
              for (int m = 0; m < 3; m++) {
                for (int n = 0; n < 10; n++) {
                  String keyText = qwerty[m][n];
                  if (upperCase) {
                    keyText.toUpperCase();
                  } else {
                    keyText.toLowerCase();
                  }
                  keys[m][n]->setText(keyText);
                  keys[m][n]->draw(); // Update the displayed button
                }
              }
            } else if (keys[i][j]->isPressed && !wasPressed[i][j]) {
              String text = keys[i][j]->getText();
              input += text;
              Serial.println(input);
              wasPressed[i][j] = true;
            } else if (!keys[i][j]->isPressed) {
              wasPressed[i][j] = false;
            }
          }
        }

        modeKey->checkTouch();
        if (modeKey->isPressed && !wasModeKeyPressed) {
          toggleMode();
          wasModeKeyPressed = true;
        } else if (!modeKey->isPressed) {
          wasModeKeyPressed = false;
        }

        spaceKey->checkTouch();
        if (spaceKey->isPressed && !wasSpaceKeyPressed) {
          input += " ";
          wasSpaceKeyPressed = true;
        } else if (!spaceKey->isPressed) {
          wasSpaceKeyPressed = false;
        }

        deleteKey->checkTouch();
        if (deleteKey->isPressed && !wasDeleteKeyPressed) {
          if (input.length() > 0) {
            input = input.substring(0, input.length() - 1);
            Serial.println(input);
            wasDeleteKeyPressed = true;
          }
        } else if (!deleteKey->isPressed) {
          wasDeleteKeyPressed = false;
        }
      }

      bool enterClicked(){
        enterKey->checkTouch();
        if (enterKey->isPressed && !wasEnterKeyPressed) {
          wasEnterKeyPressed = true;
          return true;
        } else if (!enterKey->isPressed) {
          wasEnterKeyPressed = false;
          return false;
        }
      }
};



#endif
