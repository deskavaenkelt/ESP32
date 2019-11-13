// Include the libraries we need
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <CayenneMQTTESP32.h>

//////////////////////
// Built-in sensors	//
//////////////////////
#ifdef __cplusplus
extern "C"
{
#endif
	uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

float espTemperature = 0;
int espHallSensorValue = 0;
void readDataFromEsp()
{
	espTemperature = (temprature_sens_read() - 32) / 1.8;
	espHallSensorValue = hallRead();
}

////////////////
//    WiFi    //
////////////////
const char *WIFI_NAME = "Enter_your_WiFI_SSID_here";
const char *WIFI_PASSWORD = "Enter_your_WiFI_password_here";

/////////////////
//    DHT11    //
/////////////////
#define DHTTYPE DHT11 // DHT 11
//DHT Sensor;
uint8_t DHTPin = 4;
DHT dht(DHTPin, DHTTYPE);
float Temperature = 0;
float Humidity = 0;

///////////////////
//    Cayenne    //
///////////////////
#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "Enter_Cayenne_username_here";
char password[] = "Enter_Cayenne_password_here";
char clientID[] = "Enter_Cayenne_client_ID-here";

unsigned long lastMillis = 0;

/////////////////
//    Setup    //
/////////////////
void setup()
{
	Serial.begin(115200);

	// DHT Sensor
	pinMode(DHTPin, INPUT);
	dht.begin();

	// Connect to Cayenne
	Cayenne.begin(username, password, clientID, WIFI_NAME, WIFI_PASSWORD);
}

////////////////
//    Loop    //
////////////////
void loop()
{
	Cayenne.loop();
	Temperature = dht.readTemperature();
	Humidity = dht.readHumidity();
	readDataFromEsp();

	if (millis() - lastMillis > 5000)
	{
		lastMillis = millis();

		Serial.print("Temp: ");
		Serial.print(Temperature);
		Serial.print(", ");
		Serial.print("Humidity: ");
		Serial.print(Humidity);
		Serial.print(", ");
		Serial.print("ESP Temp: ");
		Serial.println(espTemperature);

		Cayenne.virtualWrite(0, Temperature, "digital_sensor", "d");
		Cayenne.virtualWrite(1, Humidity, "digital_sensor", "d");
		Cayenne.virtualWrite(2, espTemperature, "digital_sensor", "d");
	}
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
	// Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
	//   Cayenne.virtualWrite(0, millis());
	// Some examples of other functions you can use to send data.
	//Cayenne.celsiusWrite(1, 22.0);
	//Cayenne.luxWrite(2, 700);
	//Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
