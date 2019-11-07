#include <Arduino.h>
#include <WiFi.h>

//////////////
//   WiFi   //
//////////////
#ifdef __cplusplus
extern "C"{
  #endif
  uint8_t temprature_sens_read();
  #ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

const char *ssid = "NETWORK_NAME";
const char *pass = "PASSWORD";

WiFiClient client;


//////////////
//  Touch   //
//////////////
int touch = 4;

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  // Inbuilt Temperature sensor in ESP32-Chip
  Serial.print("Temperature: ");
  // Convert RAW F to C degrees
  Serial.print((temprature_sens_read() -32) / 1.8);
  Serial.println(" C");

  // Inbuilt Hall sensor in ESP32-Chip
  int measurement = 0;
  measurement = hallRead();

  Serial.print("Hall sensor measurement: ");
  Serial.println(measurement);
  
  // Touch sensetive pins: 2, 0, 4, 15, 27, 14, 12, 13, 33, 34
  Serial.println(touchRead(touch));
  
  delay(100);
}