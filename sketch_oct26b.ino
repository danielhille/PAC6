#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// GPIO 2 D2
#define LED D0

// WiFi
const char *ssid = "Wickedbotz"; // Enter your WiFi name
const char *password = "wickedbotz";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.hivemq.com";
const char *topic = "/lampadapac";

const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    // Set software serial baud to 115200;
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(14,OUTPUT);
    // connecting to a WiFi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi.. ");
        Serial.print(ssid);
        Serial.print(password);
        digitalWrite(LED_BUILTIN, LOW);
        //digitalWrite(14, HIGH);//teste
        //digitalWrite(14, LOW);
    }
    Serial.println("Connected to the WiFi network");
    //connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
//        String client_id = "esp8266-client-";
//        client_id += String(WiFi.macAddress());
        Serial.println("Connecting to public emqx mqtt broker.....");
        if (client.connect("LAMPADAPAC")) {
            Serial.println("Public emqx mqtt broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
    // publish and subscribe
//    client.publish(topic, "hello emqx");
    client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    String message;
    for (int i = 0; i < length; i++) {
        message = message + (char) payload[i];  // convert *byte to string
    }
    Serial.print(message);
    if (message == "true") { 
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(14, LOW);  
      Serial.println("Message: LIGA LED");
      }   // LED on
    if (message == "false") { 
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(14, HIGH); 
    Serial.println("Message: DESLIGA LED"); } // LED off
    Serial.println();
    Serial.println("-----------------------");
}

void loop() {
    client.loop();
}
