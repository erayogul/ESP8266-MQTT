#include "DHT.h"    
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
   
#define DHTTYPE DHT11  
#define dht_dpin D3
#define led D0

const char* ssid = "xxxxxxx";
const char* wifi_password = "xxxxxx";

const char* mqtt_server = "34.65.22.209";
const char* mqtt_username = "eray";
const char* mqtt_password = "1234";
const char* mqtt_topic1 = "temperature";
const char* mqtt_topic2 = "humudity";
const char* clientID = "Client1";

int ledStatus = 0;

DHT dht(dht_dpin, DHTTYPE); 

WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient); // 1883 is the listener port for the Broker

void setup(void)
{ 
  dht.begin();
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  connectWifi();
  connectMqtt();
}
void loop() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();         
    char value1[8]; 
    dtostrf(t, 6, 2, value1); 
    char value2[8]; 
    dtostrf(h, 6, 2, value2); 
    publishDataa(mqtt_topic1,value1);
    publishDataa(mqtt_topic2,value2);
    digitalWrite(led, HIGH); 
    delay(350); 
    digitalWrite(led, LOW);   
    delay(100);      
    digitalWrite(led, HIGH); 
    delay(350); 
    digitalWrite(led, LOW);   
    delay(100);                   
}

void publishDataa(const char* topic, char* data){
      if (client.publish(topic, data)) {    }
      else {
        client.connect(clientID, mqtt_username, mqtt_password);
        delay(10); 
        client.publish(topic, data);
      }
}


void connectWifi(){
    WiFi.begin(ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void connectMqtt(){
  if (client.connect(clientID, mqtt_username, mqtt_password)) {
  }
  delay(700);
}
