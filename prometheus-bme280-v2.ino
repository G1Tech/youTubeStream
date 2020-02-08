#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; 

unsigned long delayTime;

const char* ssid = "NMzoAB8g";
const char* password = "g!h&DJfz1&gNt@2m9";

ESP8266WebServer server(80);

String GenerateMetrics() {
  String message = "";

  message += "# HELP bme280_measuring_temperature Current sensor temperature in celsius.\n";
  message += "# TYPE bme280_measuring_temperature gauge\n";
  message += "bme280_measuring_temperature ";
  message += bme.readTemperature();
  message += "\n";

  message += "# HELP bme280_measuring_humidity Current sensor humidity in persent.\n";
  message += "# TYPE bme280_measuring_humidity gauge\n";
  message += "bme280_measuring_humidity ";
  message += bme.readHumidity();
  message += "\n";

  message += "# HELP bme280_measuring_pressure Current sensor Pressure in mmHg.\n";
  message += "# TYPE bme280_measuring_pressure gauge\n";
  message += "bme280_measuring_pressure ";
  message += bme.readPressure() * 0.007500617;
  message += "\n";
  

  return message;
}


void HandleRoot() {
  server.send(200, "text/plain", GenerateMetrics() );
}
void HandleNotFound() {
  String message = "Error\n\n";
  server.send(404, "text/html", message);
}



void setup() {
    Serial.begin(9600);
    while(!Serial);  

// BME280
    Serial.println(F("BME280 test"));
    unsigned status;
    status = bme.begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1);
    }
    
    Serial.println("-- Default Test --");
    delayTime = 1000;

  //Setup WIFI
  WiFi.begin(ssid, password);
  Serial.println("");

  //Wait for WIFI connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());

  server.on("/", HandleRoot);
  server.on("/metrics", HandleRoot);
  server.onNotFound( HandleNotFound );

  server.begin();
  Serial.println("HTTP server started at ip " + WiFi.localIP().toString() );


    Serial.println();
}


void loop() { 
    printValues();
    server.handleClient();
    delay(delayTime);
}


void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() *0.007500617);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}
