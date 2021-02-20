#include "remoteVars.h"

void setup() {
  Serial.begin(serialRate);
  setPinModes();
  setupOLED();
  setupNetworkConnection();
  setupLookbacks();

  setFakeData();//this is temp for testing

}

void loop() {
  updateInputs();

  updateJoystickState();
  checkJoystickMovement();

  updateOLED();

  debug();
  //makeRequest();
  delay(10);
}

void setupLookbacks()
{
  for(int i = joystickLookback-1; i > 0; i--)
  {
    joystickXValues[i] = 0;
    joystickYValues[i] = 0;
  }
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
  deviceStates[2] = 1;
  numDevices = 3;
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

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true);

  display.drawBitmap(0, 0, wifi_bmp, 23, 15, SSD1306_WHITE);
  display.drawBitmap(53, 0, home_bmp, 16, 15, SSD1306_WHITE);
  display.drawBitmap(95, 0, half_bmp, 30, 15, SSD1306_WHITE);//add battery level checker

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
    int devIndex = i + selectedDevice + deviceOffset;
    String temp = deviceNames[devIndex];
    int lenOfStr = temp.length();
    int textX = 22; //if we want mid of screen64-(lenOfStr/2)*widthOfLetter;
    int textY = 16 + i * heightOfLetter;
    displayText(textX, textY, temp);
    if (deviceStates[devIndex] == 1)
    {
      displayText(2 * widthOfLetter, textY, "ON");
    }
    else
    {
      displayText(3 * widthOfLetter, textY, "OFF");
    }
    if(devIndex == selectedDevice)
    {
      display.drawBitmap(0, textY+1, arrow_bmp, 20, 15, SSD1306_WHITE);
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
  joystickLookbackPos++;
  if(joystickLookbackPos > joystickLookback -1)
    joystickLookbackPos = 0;
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
  joystickXValues[joystickLookbackPos] = joystickXValue;
  joystickXValue = averageArray(joystickXValues);
  
}

void updateJoystickY() {
  setAnalogSwitch(0, 1, 0); //update this when PCB is made
  joystickYValue = analogRead(analogInputPin);
  joystickYValues[joystickLookbackPos] = joystickYValue;
  joystickYValue = averageArray(joystickYValues);
}

void updateBattery() {
  setAnalogSwitch(0, 0, 0); //update this when PCB is made
  batteryAnalogValue = analogRead(analogInputPin);
  batteryValues[batteryLookbackPos] = batteryAnalogValue;
  batteryAnalogValue = averageArray(batteryValues);
}

int averageArray(int arr[])
{
  int tot = 0;
  int len = sizeof(arr)-1;
  for(int i = len; i > 0; i--)
  {
    tot += arr[i];
  }
  return tot/len;
}

void setAnalogSwitch(int S1, int S2, int S3) {
  digitalWrite(analogSelect1Pin, S1);
  digitalWrite(analogSelect2Pin, S2);
  digitalWrite(analogSelect3Pin, S3);
  delay(10);
}
