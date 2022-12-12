#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>


// GPIO 2 D2
#define LED D0

// WiFi

                                    //const char *ssid = "Wickedbotz"; // Enter your WiFi name
                                    //const char *password = "wickedbotz";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.hivemq.com";
const char *topic = "/lampadapac";
const int mqtt_port = 1883;

//response
boolean resRequired = false;
char *status;


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    // Set software serial baud to 115200;
    Serial.begin(115200);
    //wifi manager lib
    WiFiManager wifiManager;
    wifiManager.autoConnect("Jecabox");
    pinMode(14,OUTPUT);
    //connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
//        String client_id = "esp8266-client-";
//        client_id += String(WiFi.macAddress());
        Serial.println("Connecting to public emqx mqtt broker.....");
        if (client.connect("lampadapac")) {
            Serial.println("Conectado no broker.");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
    // publish and subscribe
    client.publish(topic, "### Jecabox online. ###");
    client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.println("---------------------------");
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    String message;
    for (int i = 0; i < length; i++) {
        message = message + (char) payload[i];  // convert *byte to string
    }
    Serial.println("---------------------------");
    Serial.print("Message: ");
    Serial.println(message);
    if (message == "true") { 
      digitalWrite(14, HIGH);  
      Serial.println("#### Message: LIGA DISPOSITIVO ####");
      }
      
    if (message == "false") { 
      digitalWrite(14, LOW); 
    Serial.println("#### Message: DESLIGA DISPOSITIVO ####"); } // LED off
    
    if (message == "pinQuery") {
      resRequired = true;
      if(digitalRead(14)==1){
        status = "HIGH"; 
      }else{
        status = "LOW";
      }
      Serial.println("---------------------------");  
      Serial.print("#### Recieved pin query, returning : ");
      Serial.print(status);
      Serial.println(" ####");
      }
}

void loop() {
    client.loop();
    if (resRequired == true){
      client.publish(topic, status);
      resRequired = false;
    }
}
