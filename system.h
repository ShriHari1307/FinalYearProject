#ifndef SYSTEM_H
#define SYSTEM_H

#include <DHT.h>

int gas = A0;
int dpin = 2;
int flame = 4;
int soil = 5;
int rain = A1;
int relay1 = 6;
int relay2 = 7;

int gasthres = 200;
int tempthres = 40;
int rainthres = 200;

#define types DHT11

DHT dht(dpin, types);

#endif
