#include "ImageDrawer.h"

ImageDrawer::ImageDrawer(Adafruit_ILI9341* tft, Adafruit_FT6206* ts, SdFat& sdInstance) : tft(tft), ts(ts), sd(sdInstance) {}

int ImageDrawer::drawImage(String imagePath, GFXcanvas16* canvas, int16_t offsetX, int16_t offsetY) {
    const uint16_t BMP_SIGNATURE = 0x4D42;
    const uint8_t BMP_HEADER_OFFSET = 18;
    const uint8_t BMP_DATA_OFFSET_LOC = 10;
    const uint8_t BMP_24BIT_DEPTH = 24;

    FsFile file = sd.open(imagePath);
    if (!file) {
        sd.errorHalt("Failed to open file");
        return 1;
    }

    // Read BMP headers
    uint16_t fileType;
    file.read(&fileType, 2);
    if (fileType != BMP_SIGNATURE) {
        Serial.println("Not a BMP file");
        file.close();
        return 2;
    }

    // Get BMP dimensions
    file.seekSet(BMP_HEADER_OFFSET);
    int32_t bmpWidth, bmpHeight;
    file.read((uint8_t*)&bmpWidth, 4);
    file.read((uint8_t*)&bmpHeight, 4);

    // Check BMP depth
    uint16_t planes, depth;
    file.read((uint8_t*)&planes, 2);
    file.read((uint8_t*)&depth, 2);
    if (depth != BMP_24BIT_DEPTH) {
        Serial.println("Only 24-bit BMP supported");
        file.close();
        return 3;
    }

    // Retrieve start of pixel data
    int32_t dataOffset;
    file.seekSet(BMP_DATA_OFFSET_LOC);
    file.read((uint8_t*)&dataOffset, 4);

    // Start reading pixel data
    file.seekSet(dataOffset);

    int padding = (4 - (bmpWidth * 3) % 4) % 4;
    
    for (int32_t y = bmpHeight - 1; y >= 0; y--) {
        for (int32_t x = 0; x < bmpWidth; x++) {
            uint8_t blue = file.read();
            uint8_t green = file.read();
            uint8_t red = file.read();

            uint16_t pixel = ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
            canvas->drawPixel(x + offsetX, y + offsetY, pixel);
        }

        // Skip padding bytes
        for (int i = 0; i < padding; i++) {
            file.read();
        }
    }

    file.close();
    return 0;
}

void ImageDrawer::getImageDimensions(String imagePath, int32_t &x, int32_t &y, int32_t &width, int32_t &height) {
    FsFile file = sd.open(imagePath);

    if (!file) {
      Serial.println("Failed to open file");
    }

    // Read BMP headers
    uint16_t fileType;
    file.read(&fileType, 2);

    if (fileType != 0x4D42) { // Check BMP signature
        Serial.println("Not a BMP file");
        file.close();
    }

    // Get image dimensions
    file.seekSet(18);
    file.read((uint8_t*)&width, 4);    
    file.read((uint8_t*)&height, 4);

    x = 0; // For BMP, the starting x is always 0
    y = 0; // For BMP, the starting y is always 0

    file.close();
}
