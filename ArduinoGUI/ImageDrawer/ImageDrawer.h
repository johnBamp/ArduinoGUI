#ifndef IMAGEDRAWER_H
#define IMAGEDRAWER_H

#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include <SdFat.h>

class ImageDrawer {
    private:
        Adafruit_ILI9341* tft;
        Adafruit_FT6206* ts;
        SdFat& sd;

    public:
        ImageDrawer(Adafruit_ILI9341* tft, Adafruit_FT6206* ts, SdFat& sdInstance);
        int drawImage(String imagePath, GFXcanvas16* canvas, int16_t offsetX, int16_t offsetY);
        void getImageDimensions(String imagePath, int32_t &x, int32_t &y, int32_t &width, int32_t &height);
};

#endif
