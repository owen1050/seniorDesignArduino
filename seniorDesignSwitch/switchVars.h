#ifndef SIWTCHVARS_H
#define SWITCHVARS_H

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
