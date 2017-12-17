#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
 
#define WIFI_AP ""
#define WIFI_PASSWORD ""
 
#define TOKEN ""
 
char thingsboardServer[] = "";
 
WiFiClient wifiClient;
 
Adafruit_BME280 bme; 
 
PubSubClient client(wifiClient);
 
int status = WL_IDLE_STATUS;
unsigned long lastSend;
 
void setup()
{
  Serial.begin(115200);
  bme.begin();
  delay(10);
  InitWiFi();
  client.setServer( thingsboardServer, 1883 );
  lastSend = 0;
}
 
void loop()
{
  if ( !client.connected() ) {
    reconnect();
  }
 
  if ( millis() - lastSend > 1000 ) { // Update and send only after 1 seconds
    getAndSendData();
    lastSend = millis();
  }
 
  client.loop();
}
 
void getAndSendData()
{
  Serial.print("getAndSendData:   ");
 
  float h = bme.readHumidity();
  float t = bme.readTemperature();
  float p = bme.readPressure()*0.007500617;
 
  String temperature = String(t);
  String humidity = String(h);
  String pressure = String(p);
 
  // Just debug messages
  Serial.print( temperature ); Serial.print( ",  " );
  Serial.print( pressure ); Serial.print( ",  " );
  Serial.println( humidity );
 
 
  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"temperature\":"; payload += temperature; payload += ",";
  payload += "\"pressure\":"; payload += pressure; payload += ",";
  payload += "\"humidity\":"; payload += humidity;
  payload += "}";
 
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  client.publish( "v1/devices/me/telemetry", attributes );
  Serial.println( attributes );
}
 
void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network
 
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}
 
 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("ESP8266 Device", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}
