#include "remoteVars.h"

void setup() {
  Serial.begin(serialRate);
  setPinModes();
  setupOLED();
  setupNetworkConnection();

}

void loop() {
  updateInputs();
  updateOLED();
  debug();
  makeRequest();
  delay(1000);
}

void makeRequest() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String serverPath = serverName;

    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("\nHTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      lastMessage = payload;
      Serial.println(payload);
    }
    else {
      Serial.print("\nError code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

void setupNetworkConnection() {
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void updateOLED() {

  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true);

  display.drawBitmap(0, 0, wifi_bmp, 23, 15, SSD1306_WHITE);
  display.drawBitmap(53, 0, home_bmp, 16, 15, SSD1306_WHITE);
  display.drawBitmap(95, 0, low_bmp, 30, 15, SSD1306_WHITE);

  displayText(0, 16, "Battery");
  temp = String(batteryAnalogValue);
  displayText(50, 16, temp);

  displayText(0, 24, "JSX");
  temp = String(joystickXValue);
  displayText(50, 24, temp);

  displayText(0, 32, "JSY");
  temp = String(joystickYValue);
  displayText(50, 32, temp);

  displayText(0, 40, "Switch");
  temp = String(switchInputValue);
  displayText(50, 40, temp);

  displayText(0, 48, "Message:");
  temp = String(lastMessage);
  displayText(54, 48, temp);

  display.display();

}

void displayText(int x, int y, String text) {
  display.setCursor(x, y);     // Start at top-left corner
  display.print(text);
}

void debug() {
  if (debugLevel == 2) {
    Serial.println("");
    Serial.print("DB1");
    Serial.print("\tBattery:"); Serial.print(batteryAnalogValue);
    Serial.print(" \tJSX:"); Serial.print(joystickXValue);
    Serial.print(" \tJSY:"); Serial.print(joystickYValue);
    Serial.print(" \tSwitch:"); Serial.print(switchInputValue);
  }
}

void setupOLED() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  display.drawBitmap(30, 16, full_screen_wifi_bmp, 60, 45, SSD1306_WHITE);
  display.display();
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
  setAnalogSwitch(0, 0, 1); //update this when PCB is made
  joystickXValue = analogRead(analogInputPin);
}

void updateJoystickY() {
  setAnalogSwitch(0, 1, 0); //update this when PCB is made
  joystickYValue = analogRead(analogInputPin);
}

void updateBattery() {
  setAnalogSwitch(0, 0, 0); //update this when PCB is made
  batteryAnalogValue = analogRead(analogInputPin);
}

void setAnalogSwitch(int S1, int S2, int S3) {
  digitalWrite(analogSelect1Pin, S1);
  digitalWrite(analogSelect2Pin, S2);
  digitalWrite(analogSelect3Pin, S3);
  delay(10);
}
