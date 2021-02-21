#include "switchVars.h"

void setup() {
  Serial.begin(115200);
  analogWriteFreq(100);
  analogWriteRange(1023);

  pinMode(analogSelectPin, OUTPUT);

  pinMode(topLEDPin, OUTPUT);
  pinMode(botLEDPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(analogInPin, INPUT);

  analogWrite(topLEDPin, 512);
  analogWrite(botLEDPin, 512);

  digitalWrite(relayPin, LOW);

  digitalWrite(analogSelectPin, LOW);
  
  setupNetworkConnection();
  checkServer();
  lastServerCheck = millis();

}

void loop()
{
  readAnalogPin();
  updateMinMax();
  updateAvgs();
  checkTriggers();
  debug();
}

void checkServer()
{
  makeGetRequest("!" + deviceName + "/" + deviceName);
  int valFromServer = lastMessage.substring(1,2).toInt();
  if (valFromServer)
  {
    digitalWrite(relayPin, HIGH);
  }
  else
  {
    digitalWrite(relayPin, LOW);
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

void updateAvgs()
{
  topAmp = topMax - topMin;
  botAmp = botMax - botMin;

  topLT = topLT * longTermAvgMult + topAmp * longTermNewMult;
  topST = topST * shortTermAvgMult + topAmp * shortTermNewMult;

  botLT = botLT * longTermAvgMult + botAmp * longTermNewMult;
  botST = botST * shortTermAvgMult + botAmp * shortTermNewMult;
}

void checkTriggers()
{
  if (topLT * topMult < topST)
  {
    topTriggerTime = millis();
  }
  if (botLT * botMult < botST)
  {
    botTriggerTime = millis();
  }

  if (millis() - topTriggerTime < topLastTime)
  {
    if (millis() - botTriggerTime < botLastTime)
    {
      if (topTriggerTime < botTriggerTime)
      {
        digitalWrite(relayPin, LOW);
        makePostRequest("set/" + deviceName + "/0");
      }
      else
      {
        digitalWrite(relayPin, HIGH);
        makePostRequest("set/" + deviceName + "/1");
      }
    }
  }
}

void readAnalogPin()
{
  inc++;
  if (inc == modulatedLookback)
  {
    inc = 0;
  }

  setAnalogSwitch(topLEDPin);
  topCurr = analogRead(A0);
  topData[inc] = topCurr;

  setAnalogSwitch(botLEDPin);
  botCurr = analogRead(A0);
  botData[inc] = botCurr;
}

void debug()
{
  if (debugMode == 1)
  {
    if (millis() - topTriggerTime < topLastTime)
    {
      if (millis() - botTriggerTime < botLastTime)
      {
        if (topTriggerTime < botTriggerTime)
        {
          Serial.println("ON");
        }
        else
        {
          Serial.println("OFF");
        }
      }
    }
  }
  
  if (debugMode == 2)
  {
    if (millis() - topTriggerTime < topLastTime)
    {
      Serial.println("TOP");
    }
    if (millis() - botTriggerTime < botLastTime)
    {
      Serial.println("BOT");
    }
  }
  
  if (debugMode == 3)
  {
    Serial.print(topST);
    Serial.print("\t");
    Serial.print(topLT);
    Serial.print("\t");
    Serial.print(botST);
    Serial.print("\t");
    Serial.println(botLT);
  }
}

void setAnalogSwitch(int pin)
{
  if (pin == topLEDPin)
  {
    digitalWrite(analogSelectPin, LOW);
  }
  if (pin == botLEDPin)
  {
    digitalWrite(analogSelectPin, HIGH);
  }
}

void updateMinMax()
{
  topMax = 0;
  topMin = 2000;

  botMax = 0;
  botMin = 2000;

  for (int i = 0; i < modulatedLookback; i++)
  {
    if (topData[i] > topMax)
    {
      topMax = topData[i];
    }
    if (topData[i] < topMin)
    {
      topMin = topData[i];
    }

    if (botData[i] > botMax)
    {
      botMax = botData[i];
    }
    if (botData[i] < botMin)
    {
      botMin = botData[i];
    }
  }
}

void makePostRequest(String path) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + path;

    http.begin(serverPath);
    int httpResponseCode = http.POST("");

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

void makeGetRequest(String path) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + path;

    http.begin(serverPath);
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
