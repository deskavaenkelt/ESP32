// Include the libraries we need
#include <WiFi.h>
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to GPIO15
#define ONE_WIRE_BUS 15
// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

DeviceAddress sensor0 = { 0x28, 0x76, 0x2C, 0x4C, 0x5, 0x0, 0x0, 0x2A };
DeviceAddress sensor1 = { 0x28, 0xD9, 0xFF, 0x4C, 0x5, 0x0, 0x0, 0x42 };
DeviceAddress sensor2 = { 0x28, 0xC7, 0xA5, 0x4D, 0x5, 0x0, 0x0, 0xAD };
float valueOfSensor0 = 0;
float valueOfSensor1 = 0;
float valueOfSensor2 = 0;


const char* WIFI_NAME= "JL-25G";  
const char* WIFI_PASSWORD = "1ar55t++";  
WiFiServer server(80);

String header;

String BLUE_LED_STATE = "off";
String RED_LED_STATE = "off";
String YELLOW_LED_STATE = "off";
String GREEN_LED_STATE = "off";


const int BLUE_LED = 25;
const int RED_LED = 26;
const int YELLOW_LED = 27;
const int GREEN_LED = 14;


void setup() {
  Serial.begin(115200);
  sensors.begin();

  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  digitalWrite(BLUE_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);


  Serial.print("Connecting to ");
  Serial.println(WIFI_NAME);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Trying to connect to Wifi Network");
  }
  Serial.println("");
  Serial.println("Successfully connected to WiFi network");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void getTemperatures(){
  // Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  // Serial.println("DONE");

  valueOfSensor0 = sensors.getTempC(sensor0);
  valueOfSensor1 = sensors.getTempC(sensor1);
  valueOfSensor2 = sensors.getTempC(sensor2);
}

void loop(){
  getTemperatures();
  WiFiClient client = server.available();   
  if (client) {                            
    Serial.println("New Client is requesting web page");          
    String current_data_line = "";               
    while (client.connected()) {            
      if (client.available()) {             
        char new_byte = client.read();             
        Serial.write(new_byte);                    
        header += new_byte;
        if (new_byte == '\n') {                    
         
          if (current_data_line.length() == 0) 
          {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("LED0=ON") != -1) 
            {
              Serial.println("GPIO23 LED is ON");
              BLUE_LED_STATE = "on";
              digitalWrite(BLUE_LED, HIGH);
            } 
            if (header.indexOf("LED0=OFF") != -1) 
            {
              Serial.println("GPIO23 LED is OFF");
              BLUE_LED_STATE = "off";
              digitalWrite(BLUE_LED, LOW);
            } 
            if (header.indexOf("LED1=ON") != -1)
            {
              Serial.println("GPIO23 LED is ON");
              RED_LED_STATE = "on";
              digitalWrite(RED_LED, HIGH);
            }
            if (header.indexOf("LED1=OFF") != -1) 
            {
              Serial.println("GPIO23 LED is OFF");
              RED_LED_STATE = "off";
              digitalWrite(RED_LED, LOW);
            }
            if (header.indexOf("LED2=ON") != -1) 
            {
              Serial.println("GPIO15 LED is ON");
              YELLOW_LED_STATE = "on";
              digitalWrite(YELLOW_LED, HIGH);
            }
            if(header.indexOf("LED2=OFF") != -1) {
              Serial.println("GPIO15 LED is OFF");
              YELLOW_LED_STATE = "off";
              digitalWrite(YELLOW_LED, LOW);
            }
            if (header.indexOf("LED3=ON") != -1) 
            {
              Serial.println("GREEN_LED is ON");
              GREEN_LED_STATE = "on";
              digitalWrite(GREEN_LED, HIGH);
            }
            if(header.indexOf("LED3=OFF") != -1) {
              Serial.println("GREEN_LED is OFF");
              GREEN_LED_STATE = "off";
              digitalWrite(GREEN_LED, LOW);
            }
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: 2px solid #4CAF50;; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");            
            // Web Page Heading
            client.println("</style></head>");
            client.println("<body><center><h1>ESP32 Web server LED controlling example</h1></center>");
            client.println("<center><h2>Web Server Example Microcontrollerslab.com</h2></center>" );
            client.println("<center><h2>Press on button to turn LED on and off</h3></center>");
            client.println("<form><center>");

            client.println("<p>BLUE LED(0) is " + BLUE_LED_STATE + "</p>");
            // If the PIN_NUMBER_22State is off, it displays the ON button      
             client.println("<center> <button class=\"button\" name=\"LED0\" value=\"ON\" type=\"submit\">Turn BLUE LED ON</button>") ;
             client.println("<button class=\"button\" name=\"LED0\" value=\"OFF\" type=\"submit\">Turn BLUE LED OFF</button><br><br>");
             
             client.println("<p>RED LED(1) is " + RED_LED_STATE + "</p>");
             client.println("<button class=\"button\" name=\"LED1\" value=\"ON\" type=\"submit\">Turn RED LED ON</button>");
             client.println("<button class=\"button\" name=\"LED1\" value=\"OFF\" type=\"submit\">Turn RED LED OFF</button> <br><br>");
             
             client.println("<p>YELLOW LED(2) is " + YELLOW_LED_STATE + "</p>");
             client.println ("<button class=\"button\" name=\"LED2\" value=\"ON\" type=\"submit\">Turn YELLOW LED ON</button>");
             client.println ("<button class=\"button\" name=\"LED2\" value=\"OFF\" type=\"submit\">Turn YELLOW LED OFF</button></center>");

             client.println("<p>GREEN LED(3) is " + GREEN_LED_STATE + "</p>");
             client.println ("<button class=\"button\" name=\"LED3\" value=\"ON\" type=\"submit\">Turn GREEN LED ON</button>");
             client.println ("<button class=\"button\" name=\"LED3\" value=\"OFF\" type=\"submit\">Turn GREEN LED OFF</button></center>");

             client.println("<h2>Sensordata from DS18B20 is</h2>");
             client.print("<p>Sensor0 is "); client.print(valueOfSensor0); client.println(" degrees Celcius</p>");
             client.print("<p>Sensor1 is "); client.print(valueOfSensor1); client.println(" degrees Celcius</p>");
             client.print("<p>Sensor2 is "); client.print(valueOfSensor2); client.println(" degrees Celcius</p>");

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