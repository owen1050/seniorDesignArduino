#ifndef SIWTCHVARS_H
#define SWITCHVARS_H

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "Test";
const char* password = "test1234";

String serverName = "http://148.76.16.24:1234/";
String deviceName = "realSwitch1";
String lastMessage = "";

long checkServerEveryMS = 1000;
long lastServerCheck = 0;

const int modulatedLookback = 50;
double topData[modulatedLookback];
double botData[modulatedLookback];
int inc = 0;

double topMax = 0;
double topMin = 0;

double botMax = 0;
double botMin = 0;

double topAmp = 0;
double botAmp = 0;

double topCurr = 0;
double topLT = 0;
double topST = 0;

double botCurr = 0;
double botLT = 0;
double botST = 0;

double longTermCount = 300;
double shortTermCount = 5;

double longTermAvgMult = ( 1 / longTermCount) * (longTermCount - 1);
double longTermNewMult = 1 / longTermCount;

double shortTermAvgMult = ( 1 / shortTermCount) * (shortTermCount - 1);
double shortTermNewMult = 1 / shortTermCount;

int counter = 0;

double topMult = 3;
double topTriggerTime = 0;
double topLastTime = 500;

double botMult = 3;
double botTriggerTime = 0;
double botLastTime = 500;

int analogSelectPin = D2;

int topLEDPin = D7;
int botLEDPin = D6;
int relayPin = D5;
int analogInPin = A0;

int debugMode = 3;

#endif
