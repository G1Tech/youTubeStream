#include <YoutubeApi.h>            // https://github.com/witnessmenow/arduino-youtube-api
#include <ArduinoJson.h>           // https://github.com/bblanchon/ArduinoJson

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

char ssid[] = "sid"; // your network SSID (name)
char password[] = "passwd";   // your network password

// google API key
// create yours: https://support.google.com/cloud/answer/6158862?hl=en
#define API_KEY "apikey"

// youtube channel ID
// find yours: https://support.google.com/youtube/answer/3250431?hl=en
#define CHANNEL_ID "UChQZ64F7jbFGPIK5-BFqV2w"

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

long subs = 0;

void setup() {

  Serial.begin(9600);

  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);


}

void loop() {

    if(api.getChannelStatistics(CHANNEL_ID))
    {
      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      Serial.println("------------------------");
      
    }
    delay(10000);
  
}
