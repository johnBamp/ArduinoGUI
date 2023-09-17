#include "Widget.h"

Widget::Widget(Adafruit_ILI9341* tft, Adafruit_FT6206* ts, SdFat& sdInstance) : tft(tft), ts(ts), sd(sdInstance) {}

extern SdFat SD;

void Widget::draw(GFXcanvas16* canvas) {
    unsigned long startTime = millis(); 
    for (int i = 0; i < style.borderWidth; i++) {
      canvas->drawRoundRect(x + i, y + i, width - 2 * i, height - 2 * i, style.radius + 1, style.borderColor);
    }

    // Fill the inside with the background color
    canvas->fillRoundRect(x + style.borderWidth, y + style.borderWidth, width - 2 * style.borderWidth, height - 2 * style.borderWidth, style.radius, style.backGroundColor);

    canvas->setFont(style.font);
    canvas->setTextSize(style.fontSize);
    canvas->setTextWrap(false);

    canvas->setTextColor(style.textColor);

    int16_t x1, y1;
    uint16_t w, h;
    canvas->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

    // Refactor alignment calculations
    int textX = 0;
    int textY = 0;

    // Rest of the code for line wrapping remains mostly unchanged, just avoid repeated function calls
    uint16_t spaceWidth;
    canvas->getTextBounds("  ", 0, 0, &x1, &y1, &w, &h);
    spaceWidth = w;
    canvas->getTextBounds(" ", 0, 0, &x1, &y1, &w, &h);
    spaceWidth -= w;

    ImageDrawer imageDrawer(tft, ts, sd);

    if (style.imagePath != "") {
      int32_t imageX, imageY, imageWidth, imageHeight;
      imageDrawer.getImageDimensions(style.imagePath, imageX, imageY, imageWidth, imageHeight);

      int imageXOffset = 0;
      int imageYOffset = 0;

      switch (style.imageAllignment) {
          case TOP_RIGHT:
          case MIDDLE_RIGHT:
          case BOTTOM_RIGHT:
              imageXOffset = canvas->width() - imageWidth - 1 - style.borderWidth;
              break;
          case TOP_CENTER:
          case MIDDLE_CENTER:
          case BOTTOM_CENTER:
              imageXOffset = (canvas->width() - imageWidth) / 2;
              break;
          case TOP_LEFT:
          case MIDDLE_LEFT:
          case BOTTOM_LEFT:
              imageXOffset = 1 + style.borderWidth;
              break;
      }

      if (style.imageAllignment >= TOP_RIGHT && style.imageAllignment <= TOP_LEFT) {
        imageYOffset = style.font ? 24 : 0;
      } else if (style.imageAllignment >= MIDDLE_RIGHT && style.imageAllignment <= MIDDLE_LEFT) {
        imageYOffset = style.font ? (canvas->height() - imageHeight) / 2 - y1 : canvas->height() / 2 - imageHeight / 2;
      } else {
        imageYOffset = (style.font ? canvas->height() - imageHeight - y1 : canvas->height() - imageHeight) - 1 - style.borderWidth;
      }

      imageDrawer.drawImage(style.imagePath, canvas, imageXOffset, imageYOffset);
    }

    //allign function
    //parse words function, return line
    //porint line
    switch (style.textAlignment) {
        case TOP_RIGHT:
        case MIDDLE_RIGHT:
        case BOTTOM_RIGHT:
            textX = 1 + style.borderWidth;
            break;
        case TOP_CENTER:
        case MIDDLE_CENTER:
        case BOTTOM_CENTER:
            textX = canvas->width() / 2 - w / 2;
            break;
        case TOP_LEFT:
        case MIDDLE_LEFT:
        case BOTTOM_LEFT:
            textX = canvas->width() - w - 1 - style.borderWidth;
            break;
    }

    if (style.textAlignment >= TOP_RIGHT && style.textAlignment <= TOP_LEFT) {
      textY = style.font ? 24 : 0;
    } else if (style.textAlignment >= MIDDLE_RIGHT && style.textAlignment <= MIDDLE_LEFT) {
      textY = style.font ? (canvas->height() - h) / 2 - y1 : canvas->height() / 2 - h / 2;
    } else {
      textY = (style.font ? canvas->height() - h - y1 : canvas->height() - h) - 1 - style.borderWidth;
    }

    int currentY = textY;
    int currentX;
    int wordStartIndex = 0;
    const int lineSpacing = 4;
    int paddingAdjustment = style.padding + style.borderWidth + 1;

    while (wordStartIndex < text.length()) {
      int lineWidth = 0;
      int spaceIndex = wordStartIndex;

      while (spaceIndex < text.length()) {
        int nextSpaceIndex = text.indexOf(' ', spaceIndex);
        if (nextSpaceIndex == -1) {
          nextSpaceIndex = text.length();
        }

        String word = text.substring(spaceIndex, nextSpaceIndex);
        canvas->getTextBounds(word.c_str(), 0, 0, &x1, &y1, &w, &h);
            
        // Check if word fits in the current line
        if (lineWidth + w > width - 2 * style.borderWidth) {
          break;
        }

        lineWidth += w + spaceWidth;
        spaceIndex = nextSpaceIndex + 1;
        }

        if (style.textAlignment == MIDDLE_LEFT || style.textAlignment == TOP_LEFT || style.textAlignment == BOTTOM_LEFT) {
          currentX = x + paddingAdjustment;  // Adjusted with padding
        } else if (style.textAlignment == MIDDLE_RIGHT || style.textAlignment == TOP_RIGHT || style.textAlignment == BOTTOM_RIGHT) {
          currentX = x + width - paddingAdjustment - lineWidth;  // Adjusted with padding
        } else { // CENTER alignments
          currentX = x + (width - lineWidth) / 2;
        }

        // Print words in the computed line
        while (wordStartIndex < spaceIndex) {
          int nextSpaceIndex = text.indexOf(' ', wordStartIndex);
          if (nextSpaceIndex == -1) {
             nextSpaceIndex = text.length();
          }

          String word = text.substring(wordStartIndex, nextSpaceIndex);
          canvas->getTextBounds(word.c_str(), 0, 0, &x1, &y1, &w, &h);

          canvas->setCursor(currentX, currentY);
          canvas->print(word);

          currentX += w + spaceWidth;

          wordStartIndex = nextSpaceIndex + 1;
        }

        currentY += h + lineSpacing;   // Move to the next line with additional spacing

        // If the new line doesn't fit in the widget, stop drawing
        if (currentY + h + paddingAdjustment > y + height) {  // Adjusted with padding
          break;
        }
    }
}

void Widget::setPosition(int newX, int newY) {
  x = newX;
  y = newY;
}

void Widget::setSize(int w, int h) {
  width = w;
  height = h;
}

void Widget::setStyle(Style newStyle){
  style = newStyle;
}

void Widget::setColumnSpan(int newColumnSpan) {
  columnSpan = newColumnSpan;
}

void Widget::setRowSpan(int newRowSpan) {
  rowSpan = newRowSpan;
}

void Widget::setText(String newText) {
  text = newText;
}

int Widget::getColumnSpan() const {
  return columnSpan;
}

int Widget::getRowSpan() const {
  return rowSpan;
}

int Widget::getX() const {
  return x;
}

int Widget::getY() const {
  return y;
}

int Widget::getWidth() const {
  return width;
}

int Widget::getHeight() const {
  return height;
}

Style Widget::getStyle() const{
  return style;
}

String Widget::getText() const{
  return text;
}
