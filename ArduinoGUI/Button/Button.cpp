#include "Button.h"

Button::Button(Adafruit_ILI9341* tft, Adafruit_FT6206* ts, SdFat& sdInstance) : Widget(tft, ts, sdInstance) {}

void Button::changeState(GFXcanvas16* canvas){
    std::swap(style.backGroundColor, style.activeColor);
    std::swap(style.textColor, style.textActiveColor);
    draw(canvas);
}

bool Button::checkTouch(GFXcanvas16* canvas) {
    if(ts->touched()) {
        TS_Point p = ts->getPoint();

        std::swap(p.x, p.y);
        p.x = map(p.x, 0, 320, 320, 0);

        if(p.x >= getX() && p.x <= getX() + getWidth() && p.y >= getY() && p.y <= getY() + getHeight()) {
            if (!isPressed) {
                Serial.println("Button pressed, and changed state");
                isPressed = true;
                changeState(canvas);
                return true;
            }
        } else if (isPressed) {
            Serial.println("Button released outside of its boundaries");
            isPressed = false;
            changeState(canvas);
            return false;
        }
    } else if(isPressed) {
        Serial.println("Button released");
        isPressed = false;
        changeState(canvas);
    }

    return false;
}

doubleTapButton::doubleTapButton(Adafruit_ILI9341* tft, Adafruit_FT6206* ts, SdFat& sdInstance) : Button(tft, ts, sdInstance) {}

bool doubleTapButton::checkTouch(GFXcanvas16* canvas) {
    if(ts->touched()) {
        TS_Point p = ts->getPoint();

        std::swap(p.x, p.y);
        p.x = map(p.x, 0, 320, 320, 0);

        unsigned long currentTime = millis();

        bool isInsideWidget = p.x >= getX() && p.x <= getX() + getWidth() && p.y >= getY() && p.y <= getY() + getHeight();

        if(isInsideWidget) {
            if (!isPressed) {  // Ensure this code runs only once per press
                isPressed = true;

                if (taps == 0) {
                    taps = 1;
                    startTime = currentTime;
                } else if (currentTime - startTime <= 1000) {
                    taps++;

                    if(taps == 2) {
                        Serial.println("Double tapped");
                        changeState(canvas);  // Change button's state on double tap
                        taps = 0;   // Reset tap count after detecting a double tap
                        doubleTapped = true;  // Set doubleTapped to true as double tap was detected
                    }
                }
            }
        } else {
            isPressed = false;  // Released the button outside of its boundaries
        }
    } else if (isPressed) {
        if (millis() - startTime > 1000 && taps == 1) {
            Serial.println("Single tap");
            taps = 0;  // Reset tap count after a timeout
        }
        isPressed = false;  // Button was released
        if (doubleTapped) {
            changeState(canvas);  // Change button's state back on release after a double tap
            doubleTapped = false;  // Reset doubleTapped after changing the state
        }
    }

    return doubleTapped;  // Return true if a double tap was detected, otherwise false
}
