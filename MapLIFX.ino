//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseRoom_ESP8266 is a sample that demo using multiple sensors
// and actuactor with the FirebaseArduino library.  
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <WiFiUdp.h>
#include <ArduinoLifx.h>

// Set these to run example.
#define FIREBASE_HOST "stl-cool.firebaseio.com"
#define FIREBASE_AUTH "SiSxa3k1kPrLpUOm0xVpRgqfRtciDAeQ0PYx2g1C"
#define WIFI_SSID "Escape618"
#define WIFI_PASSWORD "R0cketFTW"

WiFiUDP Udp;
ArduinoLifx lifx(Udp);

const int grovePowerPin = 15;
const int mag1Pin = 5;
const int analogPin = A0;
const int mag2Pin = 12;
const int mag3Pin = 14;
const int mag4Pin = 13;
char target[] = "192.168.86.50";
int  packetsSent = 0;

void setup() {
  Serial.begin(9600);
  pinMode(grovePowerPin, OUTPUT);
  digitalWrite(grovePowerPin, HIGH);

  pinMode(mag1Pin, INPUT);
  pinMode(analogPin, INPUT);
  pinMode(mag2Pin, INPUT);
  pinMode(mag3Pin, INPUT);
  pinMode(mag4Pin, INPUT);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  char targetMac[] = "d0:73:d5:12:42:01";    // broadcast MAC
  IPAddress myIp = WiFi.localIP();
  // When you're connected, print out the device's network status:
  Serial.print("IP Address: ");
  Serial.println(myIp);
  lifx.begin(myIp, target, targetMac);
  // for directed packets, tagged = 0; for broadcast, tagged =1:
  lifx.setFlags(0, 0, 1);
  // a warm glow:
  lifx.setColor(0, 100, 65535, 3500, 10);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.set("/school2/iots/mag1", false);
  Firebase.set("/school2/iots/mag2", false);
  Firebase.set("/school2/iots/mag3", false);
  Firebase.set("/school2/iots/mag4", false);
  Firebase.set("/school2/milestones/0/checkpoints/0/completed", false);
  Firebase.set("/school2/milestones/0/checkpoints/1/completed", false);
  Firebase.set("/school2/milestones/0/checkpoints/2/completed", false);
  Firebase.set("/school2/milestones/0/checkpoints/3/completed", false);
  Firebase.set("analog", 0);
}

int switch1 = 0;
int switch2 = 0;
int switch3 = 0;
int switch4 = 0;
float light = 0.0;
void loop() {
  int newMag1 = digitalRead(mag1Pin);
  if (newMag1 != switch1) {
    switch1 = newMag1;
    Firebase.setBool("/school2/iots/mag1", switch1);
    Firebase.setBool("/school2/milestones/0/checkpoints/0/completed", switch1);
    lifx.setColor(240, 65535, 65535, 3500, 3);
  }
  int newMag2 = digitalRead(mag2Pin);
  if (newMag2 != switch2) {
    switch2 = newMag2;
    Firebase.setBool("/school2/iots/mag2", switch2);
    Firebase.setBool("/school2/milestones/0/checkpoints/1/completed", switch2);
    lifx.setColor(120, 65535, 65535, 3500, 3);
  }
  int newMag3 = digitalRead(mag3Pin);
  if (newMag3 != switch3) {
    switch3 = newMag3;
    Firebase.setBool("/school2/iots/mag3", switch3);
    Firebase.setBool("/school2/milestones/0/checkpoints/2/completed", switch3);
    lifx.setColor(0, 65535, 65535, 3500, 3);
  }
  int newMag4 = digitalRead(mag4Pin);
  if (newMag4 != switch4) {
    switch4 = newMag4;
    Firebase.setBool("/school2/iots/mag4", switch4);
    Firebase.setBool("/school2/milestones/0/checkpoints/3/completed", switch4);
    lifx.setColor(60, 65535, 65535, 3500, 3);
  }
  float newLight = analogRead(analogPin);
  if (abs(newLight - light) > 100) {
    light = newLight;
    Firebase.setFloat("analog", light);
  }
  
}
