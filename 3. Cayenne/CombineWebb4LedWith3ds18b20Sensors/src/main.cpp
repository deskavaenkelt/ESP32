// Include the libraries we need
#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>

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

//////////////////////
// DS18B20 sensors	//
//////////////////////
// Data wire is connected to GPIO15
#define ONE_WIRE_BUS 15
// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

DeviceAddress sensor0 = {0x28, 0x76, 0x2C, 0x4C, 0x5, 0x0, 0x0, 0x2A};
DeviceAddress sensor1 = {0x28, 0xD9, 0xFF, 0x4C, 0x5, 0x0, 0x0, 0x42};
DeviceAddress sensor2 = {0x28, 0xC7, 0xA5, 0x4D, 0x5, 0x0, 0x0, 0xAD};
float valueOfSensor0 = 0;
float valueOfSensor1 = 0;
float valueOfSensor2 = 0;

////////////////
//    LEDs    //
////////////////
const int BLUE_LED = 25;
const int RED_LED = 26;
const int YELLOW_LED = 27;
const int GREEN_LED = 14;

String BLUE_BUTTON_STATE;
String RED_BUTTON_STATE;
String YELLOW_BUTTON_STATE;
String GREEN_BUTTON_STATE;

///////////////////
//    Cayenne    //
///////////////////
#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "Enter_Cayenne_username_here";
char password[] = "Enter_Cayenne_password_here";
char clientID[] = "Enter_Cayenne_client_ID_here";

unsigned long lastMillis = 0;

/////////////////
//    SETUP    //
/////////////////
void setup()
{
	Serial.begin(115200);

	// DS20B18 Sensors
	sensors.begin();

	// LEDs
	pinMode(BLUE_LED, OUTPUT);
	pinMode(RED_LED, OUTPUT);
	pinMode(YELLOW_LED, OUTPUT);
	pinMode(GREEN_LED, OUTPUT);

	// Connect to Cayenne
	Cayenne.begin(username, password, clientID, WIFI_NAME, WIFI_PASSWORD);
}

// DS18B20-Sensors
void getTemperatures()
{
	// Serial.print("Requesting temperatures...");
	sensors.requestTemperatures(); // Send the command to get temperatures
	// Serial.println("DONE");

	valueOfSensor0 = sensors.getTempC(sensor0);
	valueOfSensor1 = sensors.getTempC(sensor1);
	valueOfSensor2 = sensors.getTempC(sensor2);
}

/////////////////
//    LOOP     //
/////////////////
void loop()
{
	Cayenne.loop();
	getTemperatures();
	readDataFromEsp();

	if (millis() - lastMillis > 5000)
	{
		lastMillis = millis();

		Cayenne.virtualWrite(0, valueOfSensor0, "digital_sensor", "d");
		Cayenne.virtualWrite(1, valueOfSensor1, "digital_sensor", "d");
		Cayenne.virtualWrite(2, valueOfSensor2, "digital_sensor", "d");
		Cayenne.virtualWrite(3, espTemperature, "digital_sensor", "d");
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
CAYENNE_IN(4)
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	BLUE_BUTTON_STATE = getValue.asString();
	if (BLUE_BUTTON_STATE == "1")
	{
		digitalWrite(BLUE_LED, HIGH);
	}
	else
	{
		digitalWrite(BLUE_LED, LOW);
	}
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
CAYENNE_IN(5)
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	RED_BUTTON_STATE = getValue.asString();
	if (RED_BUTTON_STATE == "1")
	{
		digitalWrite(RED_LED, HIGH);
	}
	else
	{
		digitalWrite(RED_LED, LOW);
	}
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
CAYENNE_IN(6)
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	YELLOW_BUTTON_STATE = getValue.asString();
	if (YELLOW_BUTTON_STATE == "1")
	{
		digitalWrite(YELLOW_LED, HIGH);
	}
	else
	{
		digitalWrite(YELLOW_LED, LOW);
	}
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
CAYENNE_IN(7)
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	GREEN_BUTTON_STATE = getValue.asString();
	if (GREEN_BUTTON_STATE == "1")
	{
		digitalWrite(GREEN_LED, HIGH);
	}
	else
	{
		digitalWrite(GREEN_LED, LOW);
	}
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}