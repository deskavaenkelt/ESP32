#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
/*
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 */

// Include the libraries we need

// Data wire is connected to GPIO15
#define ONE_WIRE_BUS 15
// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

DeviceAddress sensor1 = { 0x28, 0x76, 0x2C, 0x4C, 0x5, 0x0, 0x0, 0x2A };
DeviceAddress sensor2 = { 0x28, 0xD9, 0xFF, 0x4C, 0x5, 0x0, 0x0, 0x42 };
DeviceAddress sensor3 = { 0x28, 0xC7, 0xA5, 0x4D, 0x5, 0x0, 0x0, 0xAD };

void setup(void){
  Serial.begin(115200);
  sensors.begin();
}

void loop(void){ 
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  Serial.print("Sensor 1(*C): ");
  Serial.print(sensors.getTempC(sensor1)); 
  Serial.print(" Sensor 1(*F): ");
  Serial.println(sensors.getTempF(sensor1)); 
 
  Serial.print("Sensor 2(*C): ");
  Serial.print(sensors.getTempC(sensor2)); 
  Serial.print(" Sensor 2(*F): ");
  Serial.println(sensors.getTempF(sensor2)); 
  
  Serial.print("Sensor 3(*C): ");
  Serial.print(sensors.getTempC(sensor3)); 
  Serial.print(" Sensor 3(*F): ");
  Serial.println(sensors.getTempF(sensor3)); 
  
  delay(2000);
}


// Sensor0 = ROM = 28 76 2C 4C 5 0 0 2A No more addresses.
// Sensor1 = ROM = 28 D9 FF 4C 5 0 0 42 No more addresses.
// Sensor2 = ROM = 28 C7 A5 4D 5 0 0 AD No more addresses.