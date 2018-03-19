/*The MIT License (MIT)
Copyright (c) 2018 Kamil Kaleta. All right reserved.
http://theveel.com/?p=482&preview=true

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>	
#endif

#include <WiFiClient.h>

#include "AirPollution.h"
bool isSensors=false;
bool isData=false;
bool measureNotFound = true;
int notProperMeasureCnt;
int sensorIndex;

String tempId;

//#######################################
bool showOutput = true;
//#######################################

void AirPollution::getSensors(int locId){
	//pjp-api/rest/station/sensors/'.$key;
	stationSensorsIds[0]="-2";
	stationSensorsIds[1]="-3";
	isSensors=true;
	isData=false;
	sensorIndex=-1;
	doUpdate("/pjp-api/rest/station/sensors/" + (String)locId );
}

void AirPollution::getAQIindex(int locId){
	//pjp-api/rest/aqindex/getIndex/'.$key;
	isData=false;
	isSensors=false;
	doUpdate("/pjp-api/rest/aqindex/getIndex/" + (String)locId);
}

void AirPollution::getData(int sensorId){
	// pjp-api/rest/data/getData/'.$sensorid;
	measureNotFound=true;	
	notProperMeasureCnt=0;
	tempId="";
	isData=true;
	isSensors=false;
	Serial.print(">>sensorID:");
	Serial.println(sensorId);
	doUpdate("/pjp-api/rest/data/getData/" + (String)sensorId);	
}

void AirPollution::doUpdate(String url) {
  JsonStreamingParser parser;
  parser.setListener(this);
  yield();
  WiFiClient client;
  const int httpPort = 80;

  int i =0;
  while(!client.connect("api.gios.gov.pl", httpPort)){
	  delay(1000);
	  i++;
	  if(i>10)
		  ESP.restart();
		  // return;
	  Serial.println("!!! no api gios.gov.pl");
	  
  }
  delay(1);
if(showOutput){
  Serial.print("Requesting URL: ");
  Serial.println(url);
}
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: api.gios.gov.pl\r\n" +
               "Connection: close\r\n\r\n");
  int retryCounter = 0;
  yield();
  while(!client.available()) {
    delay(1000);
    retryCounter++;
    if (retryCounter > 10) {
      return;
    }
  }

  int pos = 0;
  boolean isBody = false;
  char c;

  int size = 0;
  client.setNoDelay(false);
  yield();
  while(client.connected()) {
    while((size = client.available()) > 0) {
      c = client.read();
      if (c == '{' || c == '[') {
        isBody = true;
      }
      if (isBody) {
        parser.parse(c);
      }
		  delay(1);
    }
	delay(1);
	  }
  delay(1);
}

void AirPollution::whitespace(char c) {}
void AirPollution::startDocument() {}

/////////////////////////////////////////////////////////////KEY
void AirPollution::key(String key) {
	  currentKey = String(key);
//#######################################  
if(showOutput){
     	Serial.print("currentParent: ");
 	Serial.print(currentParent);	
	Serial.print(" currentKey: ");
	Serial.println(currentKey);
}
//######################################
}

void AirPollution::value(String value) {
	
//#########################################	
if(showOutput){
	Serial.print("currentValue! ");
	Serial.println(value);
}
//########################################	

	if(isSensors){

		if (currentKey == "id"){ tempId=value;}
		
		if(currentKey == "idParam" && value=="69"){ //PM2.5
			Serial.print("pm2, id 69=");
			Serial.println(tempId);
			stationSensorsIds[0]=tempId;
		}
		if(currentKey == "idParam" && value=="3"){ //PM10
				Serial.print("pm10, id 3=");
		Serial.println(tempId);
			stationSensorsIds[1]=tempId;
		}
	}
//AQI
	if(currentParent=="stIndexLevel"){
		if(currentKey=="id"){
			Serial.print("####stIndexLevel id: ");
			Serial.println(value);
			
		}
		else if(currentKey=="indexLevelName"){
			Serial.print("####indexLevelName: ");
			Serial.println(value);
			
		}		
	}
//.AQI
if(isData){
		//SINGLE MEASURMENT
		if (currentKey =="value" && value !="null" && measureNotFound){// && measureNotFound && notProperMeasureCnt<6){
				if(showOutput){
						Serial.print("####measure: ");
						Serial.println(value);
					}		
				stationSensorsValue=value;
				measureNotFound=false;
				notProperMeasureCnt=-1;
		}
		//.SINGLE MEASURMENT	
	}
}

void AirPollution::endArray() {}
void AirPollution::startObject() {  currentParent = currentKey;}
void AirPollution::endObject() {currentParent = "";}
void AirPollution::endDocument() {}
void AirPollution::startArray() {}



 String AirPollution::getStationSensorsIds(int sensorIndex){
	 // getSensors(station);
	 return stationSensorsIds[sensorIndex];
 }
 
 String AirPollution::getDataValue(int sensorIndex){
	 getData(sensorIndex);
	 return stationSensorsValue;
	 
 }
