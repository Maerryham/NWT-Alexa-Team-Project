// This example uses an Adafruit Huzzah ESP8266
// to connect to shiftr.io.
//
// You can check on your device after a successful
// connection here: https://shiftr.io/try.
//
// by Joël Gähwiler
// https://github.com/256dpi/arduino-mqtt

#include <ESP8266WiFi.h>
#include <MQTTClient.h>

boolean light_state;
int voltage = 2;
int led = 16;

const char ssid[] = "node hacker";
const char pass[] = "ilovepython";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void connect();  // <- predefine connect() for setup()

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  client.begin("broker.shiftr.io", net);
 // client.onMessage(messageReceived);

  connect();

   pinMode(voltage, INPUT);
   pinMode(led, OUTPUT);
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  
  while (!client.connect("Team-Alexa", "Electricity-Notifier", "getlight")) {
    Serial.print(".");
    delay(1000);
  } 

  Serial.println("\nconnected!");
  
  digitalWrite(led, HIGH);

 
}  

void loop() {
  //To read the voltage value if it is high or low
  light_state = digitalRead(voltage);
  
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }


  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();


  if(light_state == HIGH){

    //Send 1 to the broker
    client.publish("Light Status: on");
   
    }else{
    
      client.publish("Light Status: off");
    }
       
   // client.publish("/hello", "world");
  }

    
}
