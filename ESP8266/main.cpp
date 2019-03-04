#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define SIGNAL 16
#define BUILTIN_LED 13

void setup(){
 pinMode(SIGNAL, INPUT);

 pinMode(BUILTIN_LED, OUTPUT);
 digitalWrite(BUILTIN_LED, 0);

 
 if(HIGH == digitalRead(SIGNAL)){
   ESP.deepSleep(0);
 }
}

void loop(){
 if(HIGH == digitalRead(SIGNAL)){
   ESP.restart();
 } else {
   int a = 0;
  for (int i = 0; i < 1000; i++) {
    a += i * a;
  }
 }
}