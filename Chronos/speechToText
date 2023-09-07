#include <WiFi.h>
#include "easyI2S.h"
#include "SdFat.h"
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <Adafruit_FT6206.h>

const char* ssid = "";
const char* password = "$";
const char* host = ""
const uint16_t port = ;

const char* EOF_MARKER = "EOF";  // End of File marker
const int CHUNK_SIZE = 4096;  // bytes

Adafruit_FT6206 ts = Adafruit_FT6206();

SdFat SD;

EasyI2S mic(25, 26, 33, 16, 16000, SD);

String sendDataToServer(String filePath){
  WiFiClient client;

  int start = millis();

  if (!client.connect(host, port)) {
    return "Client Error";
  }

  SdFile file;
  // Convert String to const char* using c_str()
  if(!file.open(filePath.c_str(), O_READ)) { 
    return "File Opening Error";
  }

  int totalSize = file.fileSize();
  int totalChunks = (totalSize + CHUNK_SIZE - 1) / CHUNK_SIZE;
  int currentChunk = 0;

  byte dataBuffer[CHUNK_SIZE];
  while (file.available()) {
    int bytesRead = file.read(dataBuffer, CHUNK_SIZE);
    client.write(dataBuffer, bytesRead);
    currentChunk++;
  }

  file.close();

  client.print(EOF_MARKER);  // Send EOF marker to Python server
  
  String serverResponse = client.readStringUntil('\n');
  if (serverResponse == "ACK") {
    String transcription = client.readStringUntil('\n');
    while (!transcription.endsWith("END_TRANSCRIPTION")) {
      transcription += client.readStringUntil('\n');
    }
    transcription.remove(transcription.length() - 16);  // Remove the "END_TRANSCRIPTION"
    client.stop();
    return transcription;
  } else {
    client.stop();
    return "File transfer failed"; // Added the missing semicolon
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  ts.begin();

  Serial.println("Connected to WiFi");

  if(!SD.begin(15, SD_SCK_MHZ(25))) {
    Serial.println(F("SD begin() failed"));
    for(;;);
  }

  Serial.println("SD card started");

  Serial.println(sendDataToServer("male.wav"));
  while (1); // Stop the loop after sending the file
}

unsigned long lastTouchTime = 0; // for debouncing
const unsigned long debounceDelay = 1000; // 50 milliseconds debounce time

bool recording = false;

void loop() {
  unsigned long currentMillis = millis(); // get the current time

  if (ts.touched() && (currentMillis - lastTouchTime > debounceDelay)) {
    lastTouchTime = currentMillis; // update the last touch time

    if (recording) {
      recording = false;
      mic.stopRecord();
      sendDataToServer("audio.wav");

      if (SD.exists("audio.wav")) {
        SD.remove("audio.wav");
      }
      
    } else {
      if (SD.exists("audio.wav")) {
        SD.remove("audio.wav");
      }
      recording = true;
      mic.startRecord("test.raw");
    }
  }

  if (recording) {
    mic.record();
  }
}
