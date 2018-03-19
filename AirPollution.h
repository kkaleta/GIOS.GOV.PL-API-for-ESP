/*The MIT License (MIT)
The MIT License (MIT)
Copyright (c) 2018 Kamil Kaleta. All right reserved.
http://theveel.com/?p=482

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

#pragma once
#include <JsonListener.h>
#include <JsonStreamingParser.h>

class AirPollution: public JsonListener {
  private:
  //air pollution
  void getData(int sensorId);
    String currentKey;
    String currentParent = "";
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
