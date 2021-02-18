#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int analogSelect1Pin = D4;
int analogSelect2Pin = D5;
int analogSelect3Pin = D6;

int analogInputPin = A0;

int switchInputPin = D7;

int switchInputValue = 0;
int joystickXValue = 0;
int joystickYValue = 0;
int batteryAnalogValue = 0;

int xMid = 691;
int xDead = 200;
int xMin = 50;
int xMax = 1000;

int yMid = 721;
int yDead = 200;
int yMin = 50;
int yMax = 1000;

int debugLevel = 2; //0 - none, 1 - minimal serial, 2- all serial, 3 - OLED

int serialRate = 115200;

String temp = "";

void setup() {
  Serial.begin(serialRate);
  setPinModes();
  setupOLED();

}

void loop() {
  updateInputs();
  updateOLED();
  debug();
  delay(100);
}

void updateOLED() {
  
  display.clearDisplay();
  
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true);
  
  displayText(0,0, "Debug");
  
  displayText(0,16, "Battery");
  temp = String(batteryAnalogValue);
  displayText(50, 16, temp); 
  
  displayText(0,24, "JSX");
  temp = String(joystickXValue);
  displayText(50, 24, temp);

  displayText(0,32, "JSY");
  temp = String(joystickYValue);
  displayText(50, 32, temp);

  displayText(0,40, "Switch");
  temp = String(switchInputValue);
  displayText(50, 40, temp);
  
  display.display();
  
}

void displayText(int x, int y, String text) {
  display.setCursor(x, y);     // Start at top-left corner
  display.print(text);
}

void debug() {
  if(debugLevel == 2){
    Serial.println("");
    Serial.print("DB1");
    Serial.print("\tBattery:");Serial.print(batteryAnalogValue);
    Serial.print(" \tJSX:");Serial.print(joystickXValue);
    Serial.print(" \tJSY:");Serial.print(joystickYValue);
    Serial.print(" \tSwitch:");Serial.print(switchInputValue);
  }
}

void setupOLED() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
}

void setPinModes() {
  pinMode(analogSelect1Pin, OUTPUT);
  pinMode(analogSelect2Pin, OUTPUT);
  pinMode(analogSelect3Pin, OUTPUT);

  pinMode(switchInputPin, INPUT);

  pinMode(analogInputPin, INPUT);
}

void updateInputs() {
  updateJoystickX();
  updateJoystickY();
  updateBattery();
  updateSwitch();
}

void updateSwitch() {
  switchInputValue = !digitalRead(switchInputPin);
}

void updateJoystickX() {
  setAnalogSwitch(0,0,1); //update this when PCB is made
  joystickXValue = analogRead(analogInputPin);
}

void updateJoystickY() {
  setAnalogSwitch(0,1,0); //update this when PCB is made
  joystickYValue = analogRead(analogInputPin);
}

void updateBattery() {
  setAnalogSwitch(0,0,0); //update this when PCB is made
  batteryAnalogValue = analogRead(analogInputPin);
}

void setAnalogSwitch(int S1, int S2, int S3) {
  digitalWrite(analogSelect1Pin, S1);
  digitalWrite(analogSelect2Pin, S2);
  digitalWrite(analogSelect3Pin, S3);
  delay(10);
}
