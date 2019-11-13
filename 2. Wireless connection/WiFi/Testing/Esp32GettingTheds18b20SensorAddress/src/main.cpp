#include <Arduino.h>
#include <OneWire.h>
/*
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 */


// Based on the OneWire library example

OneWire ds(15);  //data wire connected to GPIO15

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  byte i;
  byte addr[8];
  
  if (!ds.search(addr)) {
    Serial.println(" No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  Serial.print(" ROM =");
  for (i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }
}


// Sensor0 = ROM = 28 76 2C 4C 5 0 0 2A No more addresses.
// Sensor1 = ROM = 28 D9 FF 4C 5 0 0 42 No more addresses.
// Sensor2 = ROM = 28 C7 A5 4D 5 0 0 AD No more addresses.