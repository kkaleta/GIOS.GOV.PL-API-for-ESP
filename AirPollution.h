/**The MIT License (MIT)
The MIT License (MIT) Copyright (c) 2018 Kamil Kaleta. All right reserved. http://theveel.com/?p=482

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version. This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details. You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

This library provide access to specific air parameters, especially air pollution PM (PM - Particulate Matters), from gios.gov.pl
*/

#pragma once
#include <JsonListener.h>
#include <JsonStreamingParser.h>

class AirPollution: public JsonListener {
  private:
  //air pollution
  void getData(int sensorId);
  
	int city_aqi_index;
	String	trend="";
	String 	worsePM2station;
	int 	worsePM2value=-1;
	String 	worsePM10station;
	int 	worsePM10value=-1;
	String 	worseO3station;
	int 	worseO3value=-1;
	int		worseCOvalue=-1;
	int 	someData=0;
//best
	String bestPM2station;
	int bestPM2value=999;
	String bestPM10station;
	int bestPM10value=999;
	
	int proper_loc_aqi_id = 0;
  
  String stationSensorsValue;
  //.air pollution
    String currentKey;
    String currentParent = "";
	//czujniki
	//http://api.gios.gov.pl/pjp-api/rest/station/sensors/.idLoc
	//zwraca czujniki na wskazanej lokalizacji
	#define MAX_SENSORS 10
	String paramName;
	String paramFormula;
	String paramCode;
	String idParam;
	String sensorID;
	String stationSensorsIds[MAX_SENSORS];
	//http://api.gios.gov.pl/pjp-api/rest/aqindex/getIndex/.$idLoc
	String stIndexLevel;	//indeks aqi dla stacji pomiarowej
	String indexLevelName;
	String measuerKey;
	String measuerValue;
    void doUpdate(String url);
	
 public:
void getSensors(int locId);
void getAQIindex(int locId);
String getDataValue(int sensorId);
String getStationSensorsIds( int sensorIndex);
virtual void whitespace(char c);
virtual void startDocument();
virtual void key(String key);
virtual void value(String value);
virtual void endArray();
virtual void endObject();
virtual void endDocument();
virtual void startArray();
virtual void startObject();
};
