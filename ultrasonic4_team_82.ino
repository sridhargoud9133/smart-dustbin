#include <Servo.h>
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define DHTPIN D3    // what pin we're connected to
#define DHTTYPE DHT11   // define type of sensor DHT 11
#define ORG "9h59w9"
#define DEVICE_TYPE "ruthvik1234"
#define DEVICE_ID "12345678"
#define TOKEN "987654321"

const char* ssid = "Moturi";
const char* password = "88046143";
String command;
const int trigpin= D8;
//const int trigpin2= D8;  
const int echopin= D7;
//const int echopin2= D4; 
const int led1= D1; 
//const int led2= D6; 
char server[] = ORG "messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/Data/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);



DHT dht (DHTPIN, DHTTYPE);


Servo myservo;
int duration;  
int distance;  

void setup(){  
  pinMode(trigpin,OUTPUT);
//  pinMode(trigpin2,OUTPUT);
  pinMode(led1,OUTPUT);
//  pinMode(led2,OUTPUT);  
  pinMode(echopin,INPUT); 
  //pinMode(echopin2,INPUT); 
   myservo.attach(D2); 
   Serial.begin(115200);
 Serial.println();
 dht.begin();
 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
}
  

  //  #define DHTPIN D4    // what pin we're connected to
  //#define DHTTYPE DHT11   // define type of sensor DHT 11
   

void loop(){ 
  digitalWrite(trigpin,HIGH);  
  delay(1000);  
  digitalWrite(trigpin,LOW);
 // digitalWrite(trigpin2,HIGH);  
 // digitalWrite(trigpin2,LOW);
  
  duration=pulseIn(echopin,HIGH);  
 // duration=pulseIn(echopin2,HIGH);  
  
  distance = duration*0.034/2;
  Serial.println(distance); 

   float h = dht.readHumidity();
   float t = dht.readTemperature();
   Serial.print("humidity : ");
   Serial.print(h);
   Serial.print("%");
   Serial.print("  temperature : ");
   Serial.print(t);
   Serial.println("*C");
   int pos;
  if(distance<=3) // fill
  {
    Serial.println("dustbin is full");
    digitalWrite(led1,HIGH);
    
    myservo.write(90);              // tell servo to go to position in variable 'pos'
    delay(10);
  }// waits 15ms for the servo to reach the position
  else{
    Serial.println("dustbin is Empty");
    digitalWrite(led1,LOW);
    
    myservo.write(0);              // tell servo to go to position in variable 'pos'
    }  
    PublishData(t,h,distance);
 if (!client.loop()) {
    mqttConnect();
  
  }
delay(100);
}
void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    Serial.println();
  }
}
void initManagedDevice() {
  if (client.subscribe(topic)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}



void PublishData(float temp, float humid,int distance){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"temperature\":";
  payload += temp;
  payload+="," "\"humidity\":";
  payload += humid;
  payload+="," "\"distance\":";
  payload += distance;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
