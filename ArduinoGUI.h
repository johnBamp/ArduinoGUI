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
#include <vector>

/**
 * Grid class to represent a 2D grid on the display.
 * Helps in managing and drawing GUI components in a grid format.
 */
class Grid {
  private:
    int rows;
    int columns;
    int width;     // Width of the entire grid
    int height;    // Height of the entire grid
    int gridX;     // Starting X-coordinate of the grid on the display
    int gridY;     // Starting Y-coordinate of the grid on the display
     Adafruit_ILI9341* tft; // Pointer to the TFT display object

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
    Grid(int rows, int columns, Adafruit_ILI9341* tft, int width, int height, int x = 0, int y = 0) 
    : rows(rows), columns(columns), tft(tft), width(width), height(height), gridX(x), gridY(y) {}

    // Getters for grid attributes
    int getRows() const { return rows; }
    int getColumns() const { return columns; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getX() const { return gridX; }
    int getY() const { return gridY; }

    // Setters for grid attributes
    void setRows(int newRows) { rows = newRows; }
    void setColumns(int newColumns) { columns = newColumns; }
    void setX(int newX) { gridX = newX; }
    void setY(int newY) { gridY = newY; }
};

/**
 * Widget class represents a graphical widget on a display.
 * It provides capabilities for drawing GUI elements with text, colors, and positioning,
 * and can also integrate with a grid system for more structured layout.
 */
class Widget {
  protected:
    // Positional attributes
    int x, y;
    int width, height;

    // Display objects
    Adafruit_ILI9341* tft;
    Adafruit_FT6206* ts;

    // Grid management
    Grid* grid = nullptr;
    int gridRow;
    int gridColumn;
    int columnWidth;
    int rowHeight;

    // Color attributes
    uint16_t backgroundColor;
    uint16_t borderColor;
    uint16_t textColor;

    // Text attributes
    int textSize;
    const GFXfont* font;
    String text;
    bool isCentered;

    // Shape attributes
    int radius;
    int borderWidth;
    int topOffset = 0;

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
    Widget(int x, int y, int width, int height, uint16_t color, Adafruit_FT6206* ts, Adafruit_ILI9341* tft)
    : x(x), y(y), width(width), height(height), tft(tft), ts(ts), backgroundColor(color), borderColor(color), textColor(ILI9341_BLACK) {}

    /**
     * Draws the widget on the display using the current attributes.
     */
    virtual void draw() {
      int widgetWidth, widgetHeight, widgetX, widgetY;

      // Determine widget's dimensions based on grid if assigned
       if (grid) {
        widgetWidth = grid->getWidth() / grid->getColumns();
        widgetHeight = grid->getHeight() / grid->getRows();
        widgetX = grid->getX() + gridColumn * widgetWidth;
        widgetY = grid->getY() + gridRow * widgetHeight;
      } else {
        widgetWidth = width;
        widgetHeight = height;
        widgetX = x;
        widgetY = y;
      }

      GFXcanvas16 canvas(widgetWidth, widgetHeight);

      // Render background and border
      canvas.fillRect(widgetX, widgetY, widgetWidth, widgetHeight, borderColor);
      canvas.fillRect(widgetX + borderWidth, widgetY + borderWidth, widgetWidth - borderWidth * 2, widgetHeight - borderWidth * 2, backgroundColor);

      // Render text
      canvas.setTextColor(textColor);
      canvas.setFont(font);
      canvas.setTextSize(textSize);

      int16_t textX, textY, textWidth, textHeight;
      canvas.getTextBounds(text, 0, 0, &textX, &textY, &textWidth, &textHeight);

      if (isCentered) {
        textX = canvas.width() / 2 - textWidth / 2;
        textY = canvas.height() / 2 - textHeight / 2;
      }

      canvas.setCursor(textX, textY - topOffset);
      canvas.println(text);
      tft->drawRGBBitmap(widgetX, widgetY, canvas.getBuffer(), widgetWidth, widgetHeight);
    }

    // Grid management functions
    void assignToGrid(Grid* assignedGrid, int newRow, int newColumn) {
      grid = assignedGrid;
      x = y = width = height = -1; // Invalidate the standalone positioning and dimensions
    }

    void removeFromGrid(int newX, int newY, int newWidth, int newHeight) {
      grid = nullptr;
      x = newX;
      y = newY;
      width = newWidth;
      height = newHeight;
    }

    // Setters
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

    void setColors(uint16_t newBackgroundColor, uint16_t newBorderColor, uint16_t newTextColor) {
      backgroundColor = newBackgroundColor;
      borderColor = newBorderColor;
      textColor = newTextColor;
    }

    void setTextAttributes(String newText, int newTextSize, bool newTextIsCentered, int newTopOffset = 0, const GFXfont* newFont = nullptr) {
      text = newText;
      textSize = newTextSize;
      font = newFont;
      isCentered = newTextIsCentered;
      topOffset = newTopOffset;
    }
};

/**
 * Button class represents a clickable button on the display.
 * It provides capabilities for interaction through touch events,
 * toggling appearance based on button state, and can execute specific actions when pressed.
 */
class Button : public Widget {
  private:
    uint16_t inverseColor = ILI9341_WHITE;  // Color to toggle to when button is pressed
    bool isPressed = false;  // Track if the button is currently pressed or not

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
    Button(int x, int y, int width, int height, uint16_t color, Adafruit_FT6206* touchScreen, Adafruit_ILI9341* display)
      : Widget(x, y, width, height, color, touchScreen, display) {}

    /**
     * Check if the button has been touched and update its appearance and state.
     * 
     * @return true if the button was pressed, false otherwise.
     */
    bool checkTouched() {
      if(touch->touched()) {
        TS_Point touchPoint = touch->getPoint();
        int buttonWidth, buttonHeight, buttonX, buttonY;

        // Determine button position and dimensions
        if (grid) {
          buttonWidth = grid->getWidth() / grid->getColumns();
          buttonHeight = grid->getHeight() / grid->getRows();
          buttonX = grid->getX() + gridColumn * buttonWidth;
          buttonY = grid->getY() + gridRow * buttonHeight;
        } else {
          buttonWidth = width;
          buttonHeight = height;
          buttonX = x;
          buttonY = y;
        }

        // Swap x and y for correct orientation
        std::swap(touchPoint.x, touchPoint.y);
        touchPoint.x = map(touchPoint.x, 320, 0, 0, 320);

        // Check if touch point is within the button boundaries
        if (touchPoint.x >= buttonX && touchPoint.x <= (buttonX + buttonWidth) &&
            touchPoint.y >= buttonY && touchPoint.y <= (buttonY + buttonHeight)) {
          
          if (!isPressed) {  // If the button was not pressed previously
            toggleButtonAppearance();
            isPressed = true;
            return true;
          }
        } else if (isPressed) {  // If touch was released outside the button
          toggleButtonAppearance();
          isPressed = false;
        }
      } else if (isPressed) {  // Touch was released
        toggleButtonAppearance();
        isPressed = false;
      }

      return false;
    }

    /**
     * Set the inverse color of the button.
     * 
     * @param newInverseColor Color to set as the inverse.
     */
    void setInverseColor(uint16_t newInverseColor) {
      inverseColor = newInverseColor;
    }

    /**
     * Toggle the button's appearance between its normal and inverse colors.
     */
    void toggleButtonAppearance() {
      std::swap(backgroundColor, inverseColor);  // Swap the colors
      draw();  // Redraw the button
    }
};

/**
 * ScrollableWidget class represents a widget with scrolling content on the display.
 * It provides capabilities for rendering long content that may not fit within its boundaries
 * and allows the user to scroll through the content via touch gestures.
 */
class ScrollableWidget : public Widget {
  private:
    uint16_t inverseColor = ILI9341_WHITE;  // The color when widget is inverse
    bool isPressed = false;  // To track if the widget is currently pressed or not

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

    /**
     * Checks if the widget is touched and updates its state accordingly
     */
    void checkTouched() {
      if(ts->touched()) {
        TS_Point p = ts->getPoint();  // Get the touch point

        int widgetWidth, widgetHeight, widgetX, widgetY;

        // Determine position and size based on grid placement or default position
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

        // Adjust touch point for orientation
        std::swap(p.x, p.y);
        p.x = map(p.x, 320, 0, 0, 320);

        // Update top offset based on touch point and redraw the widget
        if(p.y >= 120){
          topOffset += 5;
          draw();
        } else if(p.y <= 120) {
          topOffset -= 5;
          draw();
        }
      }
    }
};


#endif
