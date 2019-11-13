// Include the libraries we need
#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"

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
const char *WIFI_NAME = "NETWORK";
const char *WIFI_PASSWORD = "PASSWORD";
WiFiServer server(80);
WiFiClient client;

String header;

/////////////////
//    DHT11    //
/////////////////
#define DHTTYPE DHT11 // DHT 11
//DHT Sensor;
uint8_t DHTPin = 4;
DHT dht(DHTPin, DHTTYPE);
float Temperature;
float Humidity;

/////////////////
//    Setup    //
/////////////////
void setup()
{
	Serial.begin(115200);

	// DHT Sensor
	pinMode(DHTPin, INPUT);
	dht.begin();

	// Connect to WiFi
	Serial.print("Connecting to ");
	Serial.println(WIFI_NAME);
	WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(1000);
		Serial.println("Trying to connect to Wifi Network");
	}
	Serial.println("");
	Serial.println("Successfully connected to WiFi network");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP()); 
	server.begin();
}

////////////////
//    Loop    //
////////////////
void loop()
{
	readDataFromEsp();
	Temperature = dht.readTemperature();
	Humidity = dht.readHumidity();

	client = server.available();
	if (client)
	{
		Serial.println("New Client is requesting web page");
		String current_data_line = "";
		while (client.connected())
		{
			if (client.available())
			{
				char new_byte = client.read();
				Serial.write(new_byte);
				header += new_byte;
				if (new_byte == '\n')
				{
					if (current_data_line.length() == 0)
					{
						client.println("HTTP/1.1 200 OK");
						client.println("Content-type:text/html");
						client.println("Connection: close");
						client.println();

						client.println("<!DOCTYPE html><html>");
						client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
						client.println("<link rel=\"icon\" href=\"data:,\">");
						client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
						// client.println(".button { background-color: #4CAF50; border: 2px solid #4CAF50;; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");
						client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
						// Web Page Heading
						client.println("</style></head>");
						client.println("<body><center><h1>ESP32 Web server LED on Lolin D32</h1></center>");
						client.println("<form><center>");

						// ESP32 Onboard Sensor data
						client.println("<h2>Onbord sensor data from ESP32 is:</h2>");
						client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");
						client.println("<tr><td>Temp</td><td><span class=\"sensor\">");
						client.println(espTemperature);
						client.println(" *C</span></td></tr>");

						client.println("<tr><td>Hall effect</td><td><span class=\"sensor\">");
						client.println(espHallSensorValue);
						client.println(" Units</span></td></tr>");
						client.println(" </span></td></tr></table>");

						// DHT11 Sensor data
						client.println("<h2>DHT11 sensor data is:</h2>");
						client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");
						client.println("<tr><td>Temp. Celsius</td><td><span class=\"sensor\">");
						client.println(dht.readTemperature());
						client.println(" *C</span></td></tr>");

						client.println("<tr><td>Humidity</td><td><span class=\"sensor\">");
						client.println(dht.readHumidity());
						client.println(" %</span></td></tr></table>");

						client.println("</center></form></body></html>");
						client.println();
						break;
					}
					else
					{
						current_data_line = "";
					}
				}
				else if (new_byte != '\r')
				{
					current_data_line += new_byte;
				}
			}
		}
		// Clear the header variable
		header = "";
		// Close the connection
		client.stop();
		Serial.println("Client disconnected.");
		Serial.println("");
	}
}