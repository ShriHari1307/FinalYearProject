#include "system.h"
#include "setup.h"

void setup() {
  def();
}

void loop() {
  int temp = dht.readTemperature();
  int hum = dht.readHumidity();
  int f = digitalRead(flame);
  int s = digitalRead(soil);
  int r = analogRead(rain);
  int g = analogRead(gas);

  if (s == 0 && rain < rainthres || f == 0 ) {
    digitalWrite(relay1, HIGH);
  }
  else if (temp > tempthres || g > gasthres) {
    digitalWrite(relay2, HIGH);
  }
  else {
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
  }

  String values = (String(temp) + ',' + String(hum) + ',' + String(f) + ',' + String(s) + ',' + String(r) + ',' + String(g));
  Serial.println(values);

  delay(1000);
}
