const int modulatedLookback = 300;
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

double topMult = 1.5;
double topTriggerTime = 0;
double topLastTime = 500;

double botMult = 1.5;
double botTriggerTime = 0;
double botLastTime = 500;

int analogSelectPin = D3;
int analogSelectPin2 = D2;
int analogSelectPin3 = D8;
int topLEDPin = D1;
int botLEDPin = D5;
int relayPin = D6;
int analogInPin = A0;

int debugMode = 1;

void setup() {
  Serial.begin(115200);
  analogWriteFreq(100);
  analogWriteRange(1023);

  pinMode(analogSelectPin, OUTPUT);
  pinMode(analogSelectPin2, OUTPUT);
  pinMode(analogSelectPin3, OUTPUT);
  pinMode(topLEDPin, OUTPUT);
  pinMode(botLEDPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(analogInPin, INPUT);

  analogWrite(topLEDPin, 512);
  analogWrite(botLEDPin, 512);

  digitalWrite(relayPin, LOW);

  digitalWrite(analogSelectPin, LOW);
  digitalWrite(analogSelectPin2, LOW);
  digitalWrite(analogSelectPin3, LOW);
}

void loop() {
  inc++;
  if(inc == modulatedLookback)
  {
    inc = 0;
  }

  setAnalogSwitch(topLEDPin);
  topCurr = analogRead(A0);
  topData[inc] = topCurr;

  setAnalogSwitch(botLEDPin);
  botCurr = analogRead(A0);
  botData[inc] = botCurr;

  updateMinMax();

  topAmp = topMax - topMin;
  botAmp = botMax - botMin;

  topLT = topLT * longTermAvgMult + topAmp * longTermNewMult;
  topST = topST * shortTermAvgMult + topAmp * shortTermNewMult;

  botLT = botLT * longTermAvgMult + botAmp * longTermNewMult;
  botST = botST * shortTermAvgMult + botAmp * shortTermNewMult;

  if(topLT * topMult < topST)
  {
    topTriggerTime = millis();
  }
  if(botLT * botMult < botST)
  {
    botTriggerTime = millis();
  }

  if(millis() - topTriggerTime < topLastTime)
  {
    if(millis() - botTriggerTime < botLastTime)
    {
      if(topTriggerTime < botTriggerTime)
      {
        digitalWrite(relayPin, LOW);
      }
      else
      {
        digitalWrite(relayPin, HIGH);
      }
    }
  }

  if(debugMode > 0)
  {
    if(millis() - topTriggerTime < topLastTime)
    {
      Serial.println("TOP");
    }
    if(millis() - botTriggerTime < botLastTime)
    {
      Serial.println("BOT");
    }
  }
  if(debugMode > 1)
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
  if(pin == topLEDPin)
  {
    digitalWrite(analogSelectPin, LOW);
  }
  if(pin == botLEDPin)
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
  
  for(int i = 0; i < modulatedLookback; i++)
  {
    if(topData[i] > topMax)
    {
      topMax = topData[i];
    }
    if(topData[i] < topMin)
    {
      topMin = topData[i];
    }
    
    if(botData[i] > botMax)
    {
      botMax = botData[i];
    }
    if(botData[i] < botMin)
    {
      botMin = botData[i];
    }
  }
}
