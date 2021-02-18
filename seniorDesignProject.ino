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

}

void loop()
{
  readAnalogPin();
  updateMinMax();
  updateAvgs();
  checkTriggers();
  debug();
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
      }
      else
      {
        digitalWrite(relayPin, HIGH);
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
