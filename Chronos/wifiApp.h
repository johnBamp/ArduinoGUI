#ifndef WIFIAPP_H
#define WIFIAPP_H

#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <Adafruit_FT6206.h>
#include <WiFi.h>
#include "time.h"
#include "arduinoGUI.h"
#include <Fonts/FreeSans12pt7b.h> 
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <vector>
#include <SdFat.h>
#include "RTClib.h"
#include "base.h"

class WifiApp : public BaseApp {
  private:
    Adafruit_ILI9341* tft;
    Adafruit_FT6206* ts; 

    Keyboard* keys;

    Button* username; 
    Button* password; 

    enum ActiveField {
      NONE,
      USERNAME,
      PASSWORD
    };

    ActiveField activeField = NONE;
    
    String usernameText;
    String passwordText;

    void connectToWifi() {
        // Disconnect if already connected
        if (WiFi.status() == WL_CONNECTED) {
            WiFi.disconnect();
        }

        // Convert the String to char array
        char ssid[usernameText.length() + 1];
        usernameText.toCharArray(ssid, sizeof(ssid));

        char pass[passwordText.length() + 1];
        passwordText.toCharArray(pass, sizeof(pass));

        // Print SSID and Username to Serial monitor
        Serial.print("Attempting to connect with SSID: ");
        Serial.println(ssid);
        Serial.print("Using Username: ");
        Serial.println(usernameText);  // since usernameText is already a String, we can print it directly

        Serial.println("Connecting to WiFi...");

        // Start connecting to WiFi
        WiFi.begin(ssid, pass);

        // Wait until connected or timed out
        uint32_t startTime = millis();
        while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < 15000) {
            delay(100);
            Serial.print(".");
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("Connected!");
        } else {
            Serial.println("Failed to connect. Check your credentials and try again.");
        }
    }


  public:
    WifiApp(Adafruit_ILI9341* tft, Adafruit_FT6206* ts) : tft(tft), ts(ts) {
      keys = new Keyboard(0, 120, 320, 120, ts, tft);

      username = new Button(2, 2, 316, 60, ILI9341_WHITE, ts, tft);

      password = new Button(2, 64, 316, 55, ILI9341_WHITE, ts, tft);
    }

  public:
    void appSetup() override {
      keys->draw();

      username->setDimensions(320, 60, 1, 1);
      username->setText("", 2, false);
      username->addInverseColor(ILI9341_BLACK);
      username->setColor(0x2104, 0x2104, ILI9341_WHITE);


      password->setDimensions(320, 54, 1, 1);
      password->setText("", 2, false); 
      password->addInverseColor(ILI9341_BLACK);
      password->setColor(0x2104, 0x2104, ILI9341_WHITE);

      username->draw();
      password->draw();
    }

    void appLoop() override {
        if (username->checkTouched()) {
            activeField = USERNAME;
            if (usernameText == "") {
                keys->keyboardText = "";  // Clear the previous keyboard input
            }
        } else if (password->checkTouched()) {
            activeField = PASSWORD;
            if (passwordText == "") {
                keys->keyboardText = "";  // Clear the previous keyboard input
            }
        }

        keys->checkTouch();

        if (keys->keyboardText.endsWith(">")) {
            Serial.println("enter clicked");
            connectToWifi();
            keys->keyboardText = "";  // Clear the keyboard input
            return;
        }

        // Handle deletion if "<" key is pressed
        if (keys->keyboardText.endsWith("<")) {
            if (activeField == USERNAME && usernameText.length() > 0) {
                usernameText.remove(usernameText.length() - 1);
            } else if (activeField == PASSWORD && passwordText.length() > 0) {
                passwordText.remove(passwordText.length() - 1);
            }
            keys->keyboardText = "";  // Clear the keyboard input after processing
        } else {
            // Append the keyboard text to the active field's text
            if (activeField == USERNAME) {
                usernameText += keys->keyboardText;
                keys->keyboardText = "";  // Clear the keyboard input after processing
            } else if (activeField == PASSWORD) {
                passwordText += keys->keyboardText;
                keys->keyboardText = "";  // Clear the keyboard input after processing
            }
        }

        // Display the active field's text
        if (activeField == USERNAME) {
            username->setText(usernameText.c_str(), 2, false);
            username->draw();
        } else if (activeField == PASSWORD) {
            password->setText(passwordText.c_str(), 2, false);
            password->draw();
        }
    }




};

#endif
