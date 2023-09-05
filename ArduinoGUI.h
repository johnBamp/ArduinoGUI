#ifndef ARDUINOGUI_H
#define ARDUINOGUI_H

#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <Adafruit_FT6206.h>
#include <vector>
#include <functional> // At the beginning of your file

/**
 * Grid class to represent a 2D grid on the display.
 * Helps in managing and drawing GUI components in a grid format.
 */
class Grid {
  private:
    int rows;
    int columns;

    int width;  
    int height; 
      
    int gridX; 
    int gridY;  
      
    Adafruit_ILI9341* tft;

    public:
      /**
      * Constructor to initialize the grid.
      * 
      * @param rows Number of rows in the grid.
      * @param columns Number of columns in the grid.
      * @param tft Pointer to the display object.
      * @param width Width of the grid.
      * @param height Height of the grid.
      * @param x (Optional) X-coordinate for the grid's starting point. Defaults to 0.
      * @param y (Optional) Y-coordinate for the grid's starting point. Defaults to 0.
      */
      Grid(int rows, int columns, Adafruit_ILI9341* tft, int width, int height, int x = 0, int y = 0) : rows(rows), columns(columns), tft(tft), width(width), height(height), gridX(x), gridY(y) {}

      //Getters

      int getRows() const { return rows; }

      int getColumns() const { return columns; }

      int getWidth() const { return width; }

      int getHeight() const { return height; }

      int getX() const { return gridX; }

      int getY() const { return gridY; }

      //Setters

      void setRows(int r) { rows = r; }

      void setColumns(int c) { columns = c; }

      void setX(int x) { gridX = x; }

      void setY(int y) { gridY = y; }

};

/**
 * Widget class represents a graphical widget on a display.
 * It provides capabilities for drawing GUI elements with text, colors, and positioning,
 * and can also integrate with a grid system for more structured layout.
 */
class Widget {
  protected:
    //Positional Variables
    int x, y;
    int width, height;

    //Screen variables
    Adafruit_ILI9341* tft;
    Adafruit_FT6206* ts;

    //Grid variables, this allows you to manage complex GUIs easier by snapping individual elements to rows / collumns
    Grid* grid = nullptr;

    int row;
    int collumn;

    int collumnWidth;
    int rowHeight;

    //Colors, default to main color, can be set individually using setColor() function
    uint16_t backGroundColor;
    uint16_t borderColor;
    uint16_t textColor;

    //Text settings
    int textSize;
    const GFXfont* font;
    String text;
    bool centered;

    //Shape options 
    int radius;
    int borderWidth;

    int topOffset = 0;
      
    typedef std::function<void(GFXcanvas16&)> FunctionType;
    FunctionType funcPtr = nullptr;

    public:
      /**
      * Constructor for the Widget class.
      * 
      * @param x Initial x position of the widget.
      * @param y Initial y position of the widget.
      * @param width Initial width of the widget.
      * @param height Initial height of the widget.
      * @param color Initial color of the widget.
      * @param touch Pointer to the touch screen interface.
      * @param display Pointer to the display interface.
      */
      Widget(int x, int y, int width, int height, uint16_t color, Adafruit_FT6206* ts, Adafruit_ILI9341* tft) : x(x), y(y), width(width), height(height), tft(tft), ts(ts) {
        backGroundColor = color;
        borderColor = color;
        textColor = ILI9341_WHITE;
      }

      virtual void draw() {
        if(!tft or !ts){
          Serial.print(F("Failed to get ts, tft"));
          return;
        }
        int widgetWidth, widgetHeight, widgetX, widgetY;

        // Determine widget dimensions
        if (grid != nullptr) {
          widgetWidth = grid->getWidth() / grid->getColumns();
          widgetHeight = grid->getHeight() / grid->getRows();
          widgetX = grid->getX() + collumn * widgetWidth;
          widgetY = grid->getY() + row * widgetHeight;
        } else {
          widgetWidth = width;
          widgetHeight = height;
          widgetX = x;
          widgetY = y;
        }

        GFXcanvas16 canvas(widgetWidth, widgetHeight);

        // Draw background and border
        canvas.fillRoundRect(0, 0, widgetWidth, widgetHeight, radius, borderColor);
        canvas.fillRoundRect(borderWidth, borderWidth, widgetWidth - borderWidth * 2, widgetHeight - borderWidth * 2, radius, backGroundColor);

        canvas.setTextColor(textColor);
        if(font){
          canvas.setFont(font);
        }
        canvas.setTextSize(textSize);

        // Calculate text bounds
        int16_t x1, y1;
        uint16_t w, h;

        int lineLength = 0;

        canvas.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
          
        int textX = 0;
        int textY = 24;

        if(centered){
          textX = canvas.width() / 2 - w / 2;
          textY = font ? (canvas.height() - h) / 2 - y1 : canvas.height() / 2 - h / 2;
        }

        if (funcPtr) {
          funcPtr(canvas);
        }

        canvas.setCursor(textX, textY - topOffset);
        canvas.println(text);

        //canvas.drawLine(0, 120, 320, 120, ILI9341_WHITE);
        //canvas.drawLine(160, 0, 160, 240, ILI9341_WHITE);

        tft->drawRGBBitmap(widgetX, widgetY, canvas.getBuffer(), widgetWidth, widgetHeight);
      }

      //Grid functions
      void assignGrid(Grid* newGrid, int row, int collumn) {
        grid = newGrid;
        x = -1;
        y = -1;
        width = -1;
        height = -1;
      }

      void deleteGrid(int newX, int newY, int newWidth, int newHeight) {
        if(grid != nullptr){
          grid = nullptr;
        }

        x = newX;
        y = newY;
        width = newWidth;
        height = newHeight;
      }

      //Default setters
      void setPosition(int newX, int newY) {
        x = newX;
        y = newY;
      }
    
      void setDimensions(int newWidth, int newHeight, int newRadius, int newBorderWidth) {
        width = newWidth;
        height = newHeight;
        radius = newRadius;
        borderWidth = newBorderWidth;
      }

      void setColor(uint16_t newBackGroundColor,uint16_t newBorderColor,uint16_t newTextColor) {
        backGroundColor = newBackGroundColor;
        borderColor = newBorderColor;
        textColor = newTextColor;
      }

      void setText(String newText, int newTextSize, bool isCentered, int newTopOffset = 0, const GFXfont* newFont = nullptr) {
        text = newText;
        textSize = newTextSize;
        font = newFont;
        centered = isCentered;
        topOffset = newTopOffset;
      }

      void setFunction(FunctionType func) {
          this->funcPtr = func;
      }

      int getX(){ return x; }
      int getY(){ return y; }

      String getText(){ return text; }
};

/**
 * Button class represents a clickable button on the display.
 * It provides capabilities for interaction through touch events,
 * toggling appearance based on button state, and can execute specific actions when pressed.
 */
class Button : public Widget {
  private:
    uint16_t inverseColor = ILI9341_WHITE;
    bool isPressed = false;  // to track if button is currently pressed or not

  public:
    /**
     * Constructor for the Button class.
     * 
     * @param x Initial x position of the button.
     * @param y Initial y position of the button.
     * @param width Initial width of the button.
     * @param height Initial height of the button.
     * @param color Initial color of the button.
     * @param touchScreen Pointer to the touch screen interface.
     * @param display Pointer to the display interface.
     */
      Button(int x, int y, int width, int height, uint16_t color, Adafruit_FT6206* ts, Adafruit_ILI9341* tft)
      : Widget(x, y, width, height, color, ts, tft) {
        backGroundColor = color;
        borderColor = color;
        textColor = ILI9341_WHITE;
      }

      bool checkTouched() {
        if(ts->touched()) {
          TS_Point p = ts->getPoint();

          int widgetWidth, widgetHeight, widgetX, widgetY;

          // Determine position and size
          if (grid) {
            widgetWidth = grid->getWidth() / grid->getColumns();
            widgetHeight = grid->getHeight() / grid->getRows();
            widgetX = grid->getX() + collumn * widgetWidth;
            widgetY = grid->getY() + row * widgetHeight;
          } else {
            widgetWidth = width;
            widgetHeight = height;
            widgetX = x;
            widgetY = y;
          }

          int swap = p.y;
          p.y = p.x;
          p.x = swap;
          p.x = map(p.x, 320, 0, 0, 320);

          if (p.x >= widgetX && p.x <= (widgetX + widgetWidth) &&
            p.y >= widgetY && p.y <= (widgetY + widgetHeight)) {
            if (!isPressed) {  // button was not pressed previously
              toggleButtonAppearance();
              isPressed = true;
              return true;
            }
          }else if (isPressed) {  // touch released
            toggleButtonAppearance();
            isPressed = false;
          }
        } else if (isPressed) {  // touch released
          toggleButtonAppearance();
          isPressed = false;
        }
        return false;
      }

      void addInverseColor(uint16_t newInverse) {
        inverseColor = newInverse;
      }

      void toggleButtonAppearance() {
        uint16_t tmp = backGroundColor;
        backGroundColor = inverseColor;
        inverseColor = tmp;
        draw();
      }
};

/**
 * ScrollableWidget class represents a widget with scrolling content on the display.
 * It provides capabilities for rendering long content that may not fit within its boundaries
 * and allows the user to scroll through the content via touch gestures.
 */
class ScrollableWidget : public Widget {
    private:
      uint16_t inverseColor = ILI9341_WHITE;
      bool isPressed = false;  // to track if button is currently pressed or not

    public:
      /**
      * Constructor for ScrollableWidget
      *
      * @param x The x position of the widget
      * @param y The y position of the widget
      * @param width The width of the widget
      * @param height The height of the widget
      * @param color The default color of the widget
      * @param ts Pointer to the touch screen controller
      * @param tft Pointer to the screen display controller
      */
      ScrollableWidget(int x, int y, int width, int height, uint16_t color, Adafruit_FT6206* ts, Adafruit_ILI9341* tft)
      : Widget(x, y, width, height, color, ts, tft) {}

      void checkTouched() {
        if(ts->touched()) {
          TS_Point p = ts->getPoint();

          int widgetWidth, widgetHeight, widgetX, widgetY;

          // Determine position and size
          if (grid) {
            widgetWidth = grid->getWidth() / grid->getColumns();
            widgetHeight = grid->getHeight() / grid->getRows();
            widgetX = grid->getX() + collumn * widgetWidth;
            widgetY = grid->getY() + row * widgetHeight;
          } else {
            widgetWidth = width;
            widgetHeight = height;
            widgetX = x;
            widgetY = y;
          }

          int swap = p.y;
          p.y = p.x;
          p.x = swap;
          p.x = map(p.x, 320, 0, 0, 320);

          if(p.y >= 120){
            topOffset += 5;
            draw();
          }else if(p.y <= 120){
            topOffset -= 5;
            draw();
          }

              
        }

      }
};

class Keyboard {
  private:
    Adafruit_ILI9341* tft;
    Adafruit_FT6206* ts;

    static const int ROWS = 3;
    static const int COLS = 10;
    Button* buttons[ROWS][COLS];

    const char* normalKeys[ROWS][COLS] = {{"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"},
                                          {"A", "S", "D", "F", "G", "H", "J", "K", "L", "<"},
                                          {"^", "Z", "X", "C", " ", "V", "B", "N", "M", ">"}};

    void updateKeyTexts() {
      for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
          if (isUppercase) {
            String keyText = String(normalKeys[i][j]);
            keyText.toLowerCase();
            buttons[i][j]->setText(keyText.c_str(), 2, true);
          } else {
            String keyText = String(normalKeys[i][j]);
            keyText.toUpperCase();
            buttons[i][j]->setText(keyText.c_str(), 2, true);
          }
        }
      }
    }


    String text;

    Button* keys;

    int xPos;
    int yPos;
    
    int width;
    int height;

    bool isUppercase = true;

  public:

    bool enterPressed;
    String keyboardText;

    Keyboard(int x, int y, int width, int height, Adafruit_FT6206* ts, Adafruit_ILI9341* tft) : xPos(x), yPos(y), width(width), height(height), tft(tft), ts(ts) {
      int buttonWidth = width / COLS;
      int buttonHeight = height / ROWS;

      for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            buttons[i][j] = new Button(x + j * buttonWidth, y + i * buttonHeight, buttonWidth, buttonHeight, ILI9341_WHITE, ts, tft);
            buttons[i][j]->setText(normalKeys[i][j], 2, true);
            buttons[i][j]->addInverseColor(ILI9341_BLACK);
            buttons[i][j]->setDimensions(buttonWidth, buttonHeight, 0, 1);
            buttons[i][j]->setColor(0x2104, 0x2104, ILI9341_WHITE);
          }
      }
    }

    void draw() {
      for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
          buttons[i][j]->draw();
        }
      }
    }

  void checkTouch() {
      for (int i = 0; i < ROWS; i++) {
          for (int j = 0; j < COLS; j++) {
              if (buttons[i][j]->checkTouched()) {
                  const char* keyText = normalKeys[i][j];
                  if (strcmp(keyText, "<") == 0) {
                      // Delete last character
                      if (keyboardText.length() > 0) {
                          keyboardText.remove(keyboardText.length() - 1);
                      }
                  } else if (strcmp(keyText, ">") == 0) {
                      enterPressed = true;
                  }else if (strcmp(keyText, "^") == 0) {
                      // Toggle the capitalization state
                      isUppercase = !isUppercase;

                      // Update the button texts to reflect the new capitalization state
                      updateKeyTexts();

                      // Redraw the keyboard
                      draw();
                  
                  } else {
                      // If it's uppercase mode and the key is a letter, capitalize it; else add as it is
                      if (isUppercase) {
                        String modifiedKeyText = String(keyText);
                        modifiedKeyText.toUpperCase();
                        keyboardText += modifiedKeyText;
                      } else {
                        String modifiedKeyText = String(keyText);
                        modifiedKeyText.toLowerCase();
                        keyboardText += modifiedKeyText;
                      }
                  }
              }
          }
      }
  }
};

#endif
