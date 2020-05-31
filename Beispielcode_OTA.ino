//Includes für OTA
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//Includes für eMalRob-FG
#include "Ticker.h"
#include <arduino.h>
#include <analogWrite.h>
#define AIN1 17    //Pindefinitionen für ESP32
#define AIN2 16
#define BIN1 4
#define BIN2 2
#define EN 27
#define Pin 39
#define Pin1 36
int s=0;
int var;
int s1=0;
int var1;
int Z1=0;
int Z=0;
int Y1=0;
int Y=0;



//WLAN-ROUTER-DATEN
const char* ssid = "..WLAN-ROUTER-NAME..";
const char* password = "..WLAN-ROUTER-PASSWORT..";



void setup() {

  /** BEGIN OTA SETUP **/

  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);
  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");
  // No authentication by default
  // ArduinoOTA.setPassword("admin");
  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /** OTA SETUP END **/

  /** BEGIN MOTOR-PINS SETUP **/

  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  pinMode(EN,OUTPUT);
  pinMode(Pin,INPUT);
  pinMode(Pin1,INPUT);
  digitalWrite(EN,HIGH);

  /** MOTOR-PINS SETUP END **/
}



void loop() {
  ArduinoOTA.handle();

  digitalWrite(EN,HIGH);    // Einschalten des Motortreibers
  analogWrite(AIN1,255);    //Rechts
  analogWrite(AIN2,HIGH);   //fährt rückwärts/ fast decay (Motor 1)
  
  analogWrite(BIN1,255);    //Links
  analogWrite(BIN2,HIGH);   //fährt vorwärts/ fast decay (Motor 2)

  delay(1000);
  analogWrite(BIN1,HIGH);
  analogWrite(BIN2,HIGH);
  analogWrite(AIN1,LOW);
  analogWrite(AIN2,LOW);
  delay(1000);
  
  analogWrite(BIN1,LOW);
  analogWrite(BIN2,LOW); 
  analogWrite(AIN1,LOW);
  analogWrite(AIN2,LOW);
  digitalWrite(EN,LOW);    // Auschalten des Motortreibers
  delay(1000);  
}
