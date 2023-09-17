#ifndef STYLE_H
#define STYLE_H

#include <Adafruit_GFX.h>

#define TOP_RIGHT 0
#define TOP_CENTER 1
#define TOP_LEFT 2

#define MIDDLE_RIGHT 3
#define MIDDLE_CENTER 4
#define MIDDLE_LEFT 5

#define BOTTOM_RIGHT 6
#define BOTTOM_CENTER 7
#define BOTTOM_LEFT 8

struct Style {
  int radius = 0;                
  int xMargin = 0;              
  int yMargin = 0;              
  int backGroundColor = 0x0000;      
  int borderColor = 0xffff;         
  int borderWidth = 1;         
  int textColor = 0xffff;          
  int fontSize = 1;             
  const GFXfont* font = nullptr;
  int padding = 0;
  int textAlignment = MIDDLE_CENTER;  
  int imageAllignment = MIDDLE_CENTER;      

  int activeColor = 0xffff;
  int textActiveColor = 0x0000;

  String imagePath = "";

  void (*activeFunction)(GFXcanvas16* canvas, const String& icon) = nullptr;

  void (*drawFunction)(GFXcanvas16* canvas) = nullptr;

  Style& operator=(const Style& other) { 
    if (this != &other) { 
      radius = other.radius;
      xMargin = other.xMargin;
      yMargin = other.yMargin;
      backGroundColor = other.backGroundColor;
      borderColor = other.borderColor;
      borderWidth = other.borderWidth;
      textColor = other.textColor;
      fontSize = other.fontSize;
      font = other.font;
      padding = other.padding;
      textAlignment = other.textAlignment;
      activeColor = other.activeColor;
      textActiveColor = other.textActiveColor;
      activeFunction = other.activeFunction;
      drawFunction = other.drawFunction;
      imagePath = other.imagePath;
      imageAllignment = other.imageAllignment;
    }
    return *this;
  }
};



#endif
