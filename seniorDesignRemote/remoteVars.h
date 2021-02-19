#ifndef REMOTEVARS_H
#define REMOTEVARS_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
//the first 15 are yellow, the rest are blue

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "Test";
const char* password = "test1234";

String serverName = "http://148.76.16.24:1234";

int analogSelect1Pin = D4;
int analogSelect2Pin = D5;
int analogSelect3Pin = D6;

int analogInputPin = A0;

int switchInputPin = D7;

int switchInputValue = 0;
int joystickXValue = 0;
int joystickYValue = 0;
int batteryAnalogValue = 0;

int xState = 0;
int xPrevState = 0;
int xMid = 691;
int xDead = 200;
int xMin = 50;
int xMax = 1000;

int yState = 0;
int yPrevState = 0;
int yMid = 721;
int yDead = 200;
int yMin = 50;
int yMax = 1000;

int debugLevel = 2; //0 - none, 1 - minimal serial, 2- all serial, 3 - OLED

int serialRate = 115200;

String temp = "";
String lastMessage = "";

static const unsigned char PROGMEM wifi_bmp[] =
{ 0x00, 0x78, 0x00, 0x07, 0xff, 0x80, 0x1f, 0xff, 0xe0, 0x3f, 0x03, 0xf8, 0x7c, 0x00, 0x7c, 0x70,
  0x00, 0x1c, 0x20, 0x7c, 0x08, 0x01, 0xff, 0x00, 0x07, 0xff, 0x80, 0x07, 0x83, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM home_bmp[] =
{ 0x01, 0x80, 0x13, 0xc0, 0x3f, 0xe0, 0x3e, 0x70, 0x3c, 0x38, 0x38, 0x1c, 0x70, 0x0e, 0x60, 0x06,
  0x00, 0x00, 0x10, 0x08, 0x30, 0x0c, 0x30, 0x0c, 0x30, 0x0c, 0x3f, 0xfc, 0x1f, 0xfc
};

static const unsigned char PROGMEM full_bmp[] =
{ 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xe0, 0x60, 0x00, 0x00, 0x30, 0x6f, 0x3c, 0xe7, 0x90,
  0x6f, 0x3d, 0xf7, 0x90, 0x6f, 0x3d, 0xf7, 0x98, 0x6f, 0x3d, 0xf7, 0x98, 0x6f, 0x3d, 0xf7, 0x98,
  0x6f, 0x3d, 0xf7, 0x98, 0x6f, 0x3d, 0xf7, 0x98, 0x6f, 0x3d, 0xf7, 0x90, 0x6f, 0x3c, 0xe7, 0x90,
  0x60, 0x00, 0x00, 0x30, 0x3f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM half_bmp[] =
{ 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xe0, 0x60, 0x00, 0x00, 0x30, 0x6f, 0x3c, 0x00, 0x10,
  0x6f, 0x3c, 0x00, 0x10, 0x6f, 0x3c, 0x00, 0x18, 0x6f, 0x3c, 0x00, 0x18, 0x6f, 0x3c, 0x00, 0x18,
  0x6f, 0x3c, 0x00, 0x18, 0x6f, 0x3c, 0x00, 0x18, 0x6f, 0x3c, 0x00, 0x10, 0x6f, 0x3c, 0x00, 0x10,
  0x60, 0x00, 0x00, 0x30, 0x3f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM low_bmp[] =
{ 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xe0, 0x60, 0x00, 0x01, 0xb0, 0x7e, 0x00, 0x07, 0x90,
  0x6f, 0xc0, 0x1f, 0x10, 0x63, 0xf0, 0xfc, 0x18, 0x60, 0x7f, 0xe0, 0x18, 0x60, 0x1f, 0xc0, 0x18,
  0x60, 0x7f, 0xf0, 0x18, 0x63, 0xe0, 0x7e, 0x18, 0x6f, 0x80, 0x0f, 0x90, 0x6e, 0x00, 0x03, 0x10,
  0x20, 0x00, 0x00, 0x30, 0x3f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM full_screen_wifi_bmp[] = 
{0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x7f, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 
0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 
0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 
0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x07, 0xff, 0xff, 0x80, 0x1f, 0xff, 0xfe, 0x00, 
0x1f, 0xff, 0xf8, 0x00, 0x01, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xc0, 0x00, 0x00, 0x3f, 0xff, 0xc0, 
0x3f, 0xff, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xe0, 0x7f, 0xfc, 0x00, 0x00, 0x00, 0x03, 0xff, 0xe0, 
0x7f, 0xf0, 0x00, 0x00, 0x00, 0x01, 0xff, 0xe0, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 
0x7f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 
0x1f, 0x00, 0x00, 0xff, 0xf0, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x07, 0xff, 0xfe, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x1f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xc0, 0x00, 0x00, 
0x00, 0x00, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 
0x00, 0x03, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 
0x00, 0x07, 0xff, 0xe0, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x07, 0xff, 0x00, 0x0f, 0xfe, 0x00, 0x00, 
0x00, 0x07, 0xfe, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x00, 0x01, 0xfc, 0x00, 0x00, 
0x00, 0x01, 0xf0, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#endif