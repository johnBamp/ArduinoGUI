#ifndef BUTTON_H
#define BUTTON_H

#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include "../Style.h"
#include "../Widget/Widget.h"

class Button : public Widget {
    protected:
        bool isPressed = false; // New member to track button state
        void changeState(GFXcanvas16* canvas);
        
    public:
        Button(Adafruit_ILI9341* tft, Adafruit_FT6206* ts, SdFat& sdInstance);
        virtual bool checkTouch(GFXcanvas16* canvas);

};

class doubleTapButton : public Button {
    protected:
        int taps = 0;
        int startTime = 0;
        void handleTouchStart();
        void handleTapRelease();

        bool doubleTapped = false;
    public:
        doubleTapButton(Adafruit_ILI9341* tft, Adafruit_FT6206* ts, SdFat& sdInstance);
        bool checkTouch(GFXcanvas16* canvas) override;
};

#endif
