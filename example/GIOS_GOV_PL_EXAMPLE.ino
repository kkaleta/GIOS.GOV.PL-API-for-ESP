/*
The MIT License (MIT)
Copyright (c) 2018 Kamil Kaleta. All right reserved.
http://theveel.com/?p=482
*/

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include "AirPollution.h"
AirPollution pollution;


//A I R  P O L L U T I O N
#define MAX_STATIONS 8
//identyfikatory stacji pomiarowych GIOS.GOV.PL
//http://theveel.com/gios_gov_pl-findall/
int stations[MAX_STATIONS] = {400, 401, 402, 10121, 10123, 10139, 10435, 10447};

//W I F I  C r e d e n t i a l s
const char* ssid     = "_ssid_";
const char* password = "_pass_";

void setup() {
   Serial.begin(115200);
  delay(10);
  // We start by connecting to a WiFi network
//  Serial.println();
//  Serial.println();
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
//    Serial.print(".");
  }
//  Serial.println("");
//  Serial.println("WiFi connected");  
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());

updatePollution();
}

void loop() {

}

void updatePollution() {
  Serial.println("\n+-----\t+-----\t+-----\t+-----\t+----------");
  Serial.println("|PM2µg\t|PM2%\t|PM10µg\t|PM10%\t|STACJA");
  Serial.println("+-----\t+-----\t+-----\t+-----\t+----------");
  
  for (int j = 0; j < MAX_STATIONS; j++) {
       
    pollution.getSensors(stations[j]);
    /////////////PM2,5
    int s = pollution.getStationSensorsIds(0).toInt(); //(0=PM2.5, 1=PM10)
    if (s > 0) {
      int pm2Value = pollution.getDataValue(s).toInt();
      Serial.print("|"+(String)pm2Value+"µg\t|");
      ///////////////pm2 percent
      Serial.print(toPercent(pm2Value, 25)+"\t|");
    }else Serial.print("|\t|\t|");

    /////////////PM10
    s = pollution.getStationSensorsIds(1).toInt(); //(int station, int sensorIndex)
    if (s > 0) {
      int pm10Value = pollution.getDataValue(s).toInt();
      ///////////////pm2 percent
      Serial.print((String)pm10Value+"µg\t|");
      Serial.print(toPercent(pm10Value, 50)+"\t|");
      //
    }else Serial.print("|\t|\t|");
Serial.print(stationName(stations[j]));
Serial.println("");
  }//.for 'stations'
  Serial.println("+-----\t+-----\t+-----\t+-----\t+----------\ngios.gov.pl");
}

String toPercent(int val, float base) {
  int i = (val / base) * 100;
  return (String)i + "%";
}

String stationName(int id) {
  switch (id) {
    case 400:
      return"Krasinskiego";
      break;
    case 401:
      return"Bujaka";
      break;
    case 402:
      return"Bulwarowa";
      break;
    case 10121:
      return"Dielta";
      break;
    case 10123:
      return"Zloty Rog";
      break;
    case 10139:
      return"Piastow";
      break;
    case 10435:
      return"Telimeny";
      break;
    case 10447:
      return"Wadow";
      break;
  }
}