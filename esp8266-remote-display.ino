#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include "SPI.h"
#include "TFT_eSPI.h"

const char* host = "display_02";
TFT_eSPI tft = TFT_eSPI();
WiFiServer telnetServer(23);
WiFiClient serverClient;
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

int xPos, Ypos, rCol, gCol, bCol, w , h;
char sFont[64];

String dataTel;
bool once = 0;

void fOrientation() {
  int r = getValue(dataTel, ',', 1).toInt();
  tft.setRotation(r);
}

void fText()
{
  String sText = getValue(dataTel, ',', 1);
  int xPos = getValue(dataTel, ',', 2).toInt();
  int yPos = getValue(dataTel, ',', 3).toInt();
  int sFont = getValue(dataTel, ',', 4).toInt();
  int rCol = getValue(dataTel, ',', 5).toInt();
  int gCol = getValue(dataTel, ',', 6).toInt();
  int bCol = getValue(dataTel, ',', 7).toInt();
  tft.setTextColor(tft.color565(rCol, gCol, bCol));
  tft.setCursor(xPos, yPos);
  tft.setTextFont(sFont);
  tft.println(sText);
}

void fDrawRect()
{
  int xPos = getValue(dataTel, ',', 1).toInt();
  int yPos = getValue(dataTel, ',', 2).toInt();
  int w = getValue(dataTel, ',', 3).toInt();
  int h = getValue(dataTel, ',', 4).toInt();
  int r = getValue(dataTel, ',', 5).toInt();
  int rCol = getValue(dataTel, ',', 6).toInt();
  int gCol = getValue(dataTel, ',', 7).toInt();
  int bCol = getValue(dataTel, ',', 8).toInt();
  tft.drawRoundRect(xPos, yPos, w, h, r, tft.color565(rCol, gCol, bCol));
}

void fFillRect()
{
  int xPos = getValue(dataTel, ',', 1).toInt();
  int yPos = getValue(dataTel, ',', 2).toInt();
  int w = getValue(dataTel, ',', 3).toInt();
  int h = getValue(dataTel, ',', 4).toInt();
  int r = getValue(dataTel, ',', 5).toInt();
  int rCol = getValue(dataTel, ',', 6).toInt();
  int gCol = getValue(dataTel, ',', 7).toInt();
  int bCol = getValue(dataTel, ',', 8).toInt();
  tft.fillRoundRect(xPos, yPos, w, h, r, tft.color565(rCol, gCol, bCol));
}

void fLine()
{
  int xPos = getValue(dataTel, ',', 1).toInt();
  int yPos = getValue(dataTel, ',', 2).toInt();
  int w = getValue(dataTel, ',', 3).toInt();
  int h = getValue(dataTel, ',', 4).toInt();
  int rCol = getValue(dataTel, ',', 5).toInt();
  int gCol = getValue(dataTel, ',', 6).toInt();
  int bCol = getValue(dataTel, ',', 7).toInt();
  tft.drawLine(xPos, yPos, w, h, tft.color565(rCol, gCol, bCol));
}

void fDrawCircle()
{
  int xPos = getValue(dataTel, ',', 1).toInt();
  int yPos = getValue(dataTel, ',', 2).toInt();
  int r = getValue(dataTel, ',', 3).toInt();
  int rCol = getValue(dataTel, ',', 4).toInt();
  int gCol = getValue(dataTel, ',', 5).toInt();
  int bCol = getValue(dataTel, ',', 6).toInt();
  tft.drawCircle(xPos, yPos, r, tft.color565(rCol, gCol, bCol));
}

void fFillCircle()
{
  int xPos = getValue(dataTel, ',', 1).toInt();
  int yPos = getValue(dataTel, ',', 2).toInt();
  int r = getValue(dataTel, ',', 3).toInt();
  int rCol = getValue(dataTel, ',', 4).toInt();
  int gCol = getValue(dataTel, ',', 5).toInt();
  int bCol = getValue(dataTel, ',', 6).toInt();
  tft.fillCircle(xPos, yPos, r, tft.color565(rCol, gCol, bCol));
}

void fScreen() {
  int rCol = getValue(dataTel, ',', 1).toInt();
  int gCol = getValue(dataTel, ',', 2).toInt();
  int bCol = getValue(dataTel, ',', 3).toInt();
  tft.fillScreen(tft.color565(rCol, gCol, bCol));
}

void fBrightnest() {
  int w = getValue(dataTel, ',', 1).toInt();
  analogWrite(D0, w);
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup() {
  analogWrite(D0, 200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(tft.color565(0, 255, 255));
  tft.setTextFont(4);
  tft.setCursor(65, 80);
  tft.println("Siklosi Electronics");
  tft.setTextFont(2);
  tft.setCursor(69, 67);
  tft.println("v");
  tft.setCursor(123, 72);
  tft.println("v");
  tft.setCursor(150, 108);
  tft.println("v1.0");
  tft.drawLine(75, 105, 260, 105, tft.color565(0, 100, 100));

  Serial.begin(115200);

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  Serial.begin(115200);
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("display");
  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    tft.setCursor(120, 122);
    tft.println(WiFi.localIP());
    telnetServer.begin();
    telnetServer.setNoDelay(true);
    httpUpdater.setup(&httpServer);
    httpServer.begin();
  }
}

  void loop() {
    httpServer.handleClient();
    if (telnetServer.hasClient()) {
      if (!serverClient || !serverClient.connected()) {
        if (serverClient) {
          serverClient.stop();
          Serial.println("Telnet Client Stop");
        }
        serverClient = telnetServer.available();
        Serial.println("New Telnet client");
        serverClient.flush();  // clear input buffer, else you get strange characters
      }
    }

    while (serverClient.available()) { // get data from Client
      char received = serverClient.read();
      if (received != 10) {
        dataTel += received;
      }
      if (received == 13) {
        //      Serial.write(dataTel);

        int cmd = getValue(dataTel, ',', 0).toInt();

        switch (cmd) {
          case 1:
            fOrientation();
            break;
          case 2:
            fText();
            break;
          case 3:
            fDrawRect();
            break;
          case 4:
            fFillRect();
            break;
          case 5:
            fLine();
            break;
          case 6:
            fDrawCircle();
            break;
          case 7:
            fFillCircle();
            break;
          case 8:
            fScreen();
            break;
          case 9:
            fBrightnest();
            break;
        }

        dataTel = "";
      }
    }
    if (serverClient && serverClient.connected() && once == 0) { // send data to Client
      //serverClient.println("display v0.1");
      once = 1;
    }

  }
