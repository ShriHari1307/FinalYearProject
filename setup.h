#ifndef SETUP_H
#define SETUP_H
void def() 
{
  		Serial.begin(9600);
 		 dht.begin();
 pinMode(gas, INPUT);
  		 pinMode(flame, INPUT);
 pinMode(soil, INPUT);
 pinMode(rain, INPUT);
 pinMode(dpin, INPUT);
 pinMode(relay1, OUTPUT);
 pinMode(relay2, OUTPUT);	
 digitalWrite(relay1, LOW);	
  		 digitalWrite(relay2, LOW);
}
#endif
