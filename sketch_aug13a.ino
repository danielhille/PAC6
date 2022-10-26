#include <ESP8266WiFi.h>
#include <SocketIoClient.h>

    void messageEventHandler(const char * payload, size_t length) {
    Serial.printf("got message: %s\n", payload);
    }
      SocketIoClient socketIO;
void setup() {
 Serial.begin(115200);
 WiFi.begin("Hille_2G", "d@ju2221");
 Serial.println("Connecting");
 while (WiFi.status() != WL_CONNECTED)
 {
  delay(500);
  Serial.print(".");
 }
 Serial.println();
 Serial.print("Connected, IP address: ");
 Serial.println(WiFi.localIP());


   // server address, port and URL
  socketIO.begin("192.168.1.2", 3005);
  // event handler for the event message

  socketIO.on("reply",messageEventHandler);

}
uint64_t messageTimestamp;
void loop() {
  socketIO.loop();
  uint64_t now = millis();
  if(now - messageTimestamp > 6000) {
    messageTimestamp = now;
    // Send event     
  socketIO.emit("message", "\"this is a message from the client\"");   
  }    
}
