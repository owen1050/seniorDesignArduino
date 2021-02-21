
#include "remoteVars.h"

void setup() {
  Serial.begin(serialRate);
  setPinModes();
  setupOLED();
  setupNetworkConnection();
  setupLookbacks();
  //setFakeData();//this is temp for testing
  checkServer();
  lastServerCheck = millis();
}

void loop() {
  updateInputs();

  updateJoystickState();
  checkJoystickMovement();

  updateOLED();
  debug();

  if(millis() - lastServerCheck > checkServerEveryMS)
  {
    checkServer();
    lastServerCheck = millis();
  }
  delay(10);
}

void checkServer()
{
  makeRequest("all");
  int cur = 1;
  int i = 0;
  int i0 = 0;
  int i1 = 0;
  while(lastMessage.indexOf(":", cur) > 0)
  {
    i0 = cur;
    i1 = lastMessage.indexOf(":", i0);
    deviceNames[i] = lastMessage.substring(i0, i1);
    i0 = i1+1;
    i1 = lastMessage.indexOf(",", i0);
    if(i1 == -1)
    {
      i1 = lastMessage.indexOf("}", i0);
    }
    deviceStates[i] = lastMessage.substring(i0, i1).toInt();
    cur = i1+1;
    i++;
  }
  numDevices = i;
}

void setupLookbacks()
{
  for(int i = batteryLookback-1; i > 0; i--)
  {
    batteryValues[i] = 0;
  }
}

void setFakeData() {
  deviceNames[0] = "Device0";
  deviceStates[0] = 0;
  deviceNames[1] = "Device1";
  deviceStates[1] = 0;
  deviceNames[2] = "Device2";
  deviceStates[2] = 0;
  deviceNames[3] = "Device3";
  deviceStates[3] = 0;
  numDevices = 4;
}

void checkJoystickMovement() {
  if (xPrevState == 0 and xState == 1)
  {
    deviceStates[selectedDevice] = !deviceStates[selectedDevice];
    Serial.println("RIGHT");
  }
  if (xPrevState == 0 and xState == -1)
  {
    deviceStates[selectedDevice] = !deviceStates[selectedDevice];
    Serial.println("LEFT");
  }
  if (yPrevState == 0 and yState == 1)
  {
    if (selectedDevice > 0)
      selectedDevice--;
    Serial.println("UP");
  }
  if (yPrevState == 0 and yState == -1)
  {
    if (selectedDevice < numDevices - 1)
      selectedDevice++;
    Serial.println("DOWN");
  }
}

void updateJoystickState() {
  xPrevState = xState;
  if (xPrevState == -1)
  {
    if (joystickXValue > (xMid - xDead))
    {
      xState = 0;
    }
    else
    {
      xState = -1;
    }
  }
  else if (xPrevState == 1)
  {
    if (joystickXValue < (xMid + xDead))
    {
      xState = 0;
    }
    else
    {
      xState = 1;
    }
  }
  else
  {
    if (joystickXValue > xMax)
    {
      xState = 1;
    }
    else if (joystickXValue < xMin)
    {
      xState = -1;
    }
    else
    {
      xState = 0;
    }
  }

  yPrevState = yState;
  if (yPrevState == -1)
  {
    if (joystickYValue > (yMid - yDead))
    {
      yState = 0;
    }
    else
    {
      yState = -1;
    }
  }
  else if (yPrevState == 1)
  {
    if (joystickYValue < (yMid + yDead))
    {
      yState = 0;
    }
    else
    {
      yState = 1;
    }
  }
  else
  {
    if (joystickYValue > yMax)
    {
      yState = 1;
    }
    else if (joystickYValue < yMin)
    {
      yState = -1;
    }
    else
    {
      yState = 0;
    }
  }
}

void makeRequest(String path) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + path;

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
      String payload = http.getString();
      Serial.println(payload);
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
  if(batteryAnalogValue > batteryMidLevel)
  {
    display.drawBitmap(95, 0, full_bmp, 30, 15, SSD1306_WHITE);//add battery level checker
  }
  else if(batteryAnalogValue > batteryLowLevel)
  {
    display.drawBitmap(95, 0, half_bmp, 30, 15, SSD1306_WHITE);//add battery level checker
  }
  else
  {
    display.drawBitmap(95, 0, low_bmp, 30, 15, SSD1306_WHITE);//add battery level checker
  }
  

  int i = 3;
  if (numDevices < 3)
    i = numDevices;

  selectorPosition = 1;
  deviceOffset = -1;
  if (selectedDevice == 0)
  {
    deviceOffset = 0;
    selectorPosition = 0;
  }
  if (selectedDevice == numDevices - 1)
  {
    deviceOffset = -2;
    selectorPosition = 2;
  }

  for (; i > 0; i--)
  {
    int devIndex = i + selectedDevice + deviceOffset -1;
    String temp = deviceNames[devIndex];
    int lenOfStr = temp.length();
    int textX = 22;
    int textY = 16 + i * heightOfLetter;
    displayText(textX, textY, temp);
    if (deviceStates[devIndex] == 1)
    {
      displayText(110, textY, "ON");
    }
    else
    {
      displayText(110, textY, "OFF");
    }
    if(devIndex == selectedDevice)
    {
      display.drawBitmap(0, textY-4, arrow_bmp, 20, 15, SSD1306_WHITE);
    }

  }
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
    Serial.print("  \tJSX:"); Serial.print(joystickXValue);
    Serial.print("   \tJSY:"); Serial.print(joystickYValue);
    Serial.print("   \tSwitch:"); Serial.print(switchInputValue);
    Serial.print(" \txState:"); Serial.print(xState);
    Serial.print(" \tyState:"); Serial.print(yState);
    Serial.print(" \tSD:"); Serial.print(selectedDevice);
    Serial.print(" \tSP:"); Serial.print(selectorPosition);
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
  
  batteryLookbackPos++;
  if(batteryLookbackPos > batteryLookback -1)
    batteryLookbackPos = 0;
    
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
  batteryValues[batteryLookbackPos] = batteryAnalogValue;
  int tot = 0;
  for(int i = 0; i < batteryLookback; i++)
  {
    tot += batteryValues[i];
  }
  batteryAnalogValue = tot/batteryLookback;
}


void setAnalogSwitch(int S1, int S2, int S3) {
  digitalWrite(analogSelect1Pin, S1);
  digitalWrite(analogSelect2Pin, S2);
  digitalWrite(analogSelect3Pin, S3);
  delay(10);
}
