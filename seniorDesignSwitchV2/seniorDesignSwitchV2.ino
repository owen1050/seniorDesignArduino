#include "switchVars.h"

void setup() {
  Serial.begin(serialSpeed);
  analogWriteFreq(analogFreq);
  analogWriteRange(analogRange);

  pinMode(analogSelectPin, OUTPUT);

  pinMode(topLEDPin, OUTPUT);
  pinMode(botLEDPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(analogInPin, INPUT);

  analogWrite(topLEDPin, analogSetValue);
  analogWrite(botLEDPin, analogSetValue);

  digitalWrite(relayPin, LOW);

  digitalWrite(analogSelectPin, LOW);

  setupNetworkConnection();
  makePostRequest("/new/" + deviceName);
  checkServer();
  lastServerCheck = millis();

}

void loop()
{
  if (millis() - lastServerCheck > checkServerEveryMS)
  {
    checkServer();
    lastServerCheck = millis();
  }
  readAnalogPin();
  checkTriggers();

  debug();
}

void checkServer()
{
  makeGetRequest("!" + deviceName + "/" + deviceName);
  int valFromServer = lastMessage.substring(1, 2).toInt();
  if (valFromServer)
  {
    digitalWrite(relayPin, HIGH);
    Serial.println("Server Says ON");
  }
  else
  {
    digitalWrite(relayPin, LOW);
    Serial.println("Server Says OFF");
  }
}

void setupNetworkConnection() {
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

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


void checkTriggers()
{
  if (topAmp > topTriggerLevel && millis() - topTriggerTime > topLastTime)
  {
    topTriggerTime = millis();
  }
  if (botAmp > botTriggerLevel && millis() - botTriggerTime > botLastTime)
  {
    botTriggerTime = millis();
  }

  if (millis() - topTriggerTime < topLastTime)
  {
    if (millis() - botTriggerTime < botLastTime)
    {
      if (millis() - lastTriggerChange > lastTriggerTime)
      {
        lastTriggerChange = millis();
        if (topTriggerTime < botTriggerTime)
        {
          digitalWrite(relayPin, LOW);
          Serial.println("OFF");
          makePostRequest("/set/" + deviceName + "/0");
        }
        else
        {
          digitalWrite(relayPin, HIGH);
          Serial.println("ON");
          makePostRequest("/set/" + deviceName + "/1");
        }
      }
    }
  }
}

void readAnalogPin()
{
  setAnalogSwitch(topLEDPin);
  delay(1);
  int temp = 0;
  int tot = 0;
  topMax = 0;
  topMin = 1024;
  for (int i = 0; i < modulatedLookback; i++)
  {
    temp = analogRead(A0);
    topData[i] = temp;
    tot = tot + temp;
    if (temp < topMin)
      topMin = temp;
    if (temp > topMax)
      topMax = temp;
  }
  topAmp = topMax - topMin;

  setAnalogSwitch(botLEDPin);
  delay(1);

  temp = 0;
  tot = 0;
  botMax = 0;
  botMin = 1024;
  for (int i = 0; i < modulatedLookback; i++)
  {
    temp = analogRead(A0);
    botData[i] = temp;
    tot = tot + temp;
    if (temp < botMin)
      botMin = temp;
    if (temp > botMax)
      botMax = temp;
  }
  botAmp = botMax - botMin;
}

void debug()
{
  if (debugMode == 1)
  {
    Serial.print(topAmp);
    Serial.print("\t");
    Serial.print(botAmp);
    Serial.println("\t");
    
  }

  if (debugMode == 2)
  {
    Serial.print(topAmp);
    Serial.print("\t");
    Serial.print(botAmp);
    Serial.print("\t");
    Serial.print(topTriggerTime);
    Serial.print("\t");
    Serial.print(botTriggerTime);
    Serial.println("\t");
    
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

void makePostRequest(String path) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = serverName + path;
    Serial.print("MadePOSTRequest: ");
    Serial.println(serverPath);
    http.begin(serverPath);
    int httpResponseCode = http.POST("");

    if (httpResponseCode > 0) {
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
    Serial.print("MadeGETRequest: ");
    Serial.println(serverPath);
    http.begin(serverPath);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      
      String payload = http.getString();
      lastMessage = payload;
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
