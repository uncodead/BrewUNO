#ifndef TemperatureService_h
#define TemperatureService_h

#include <OneWire.h>
#include <DallasTemperature.h>
#include <TimeLib.h>
#include <ESPAsyncWebServer.h>

#define APPLICATION_JSON_TYPE "application/json"
#define GET_SENSORS_SERVICE_PATH "/rest/getsensors"

class TemperatureService
{
public:
  TemperatureService(AsyncWebServer *server, FS *fs, DallasTemperature dallasTemperature);
  ~TemperatureService();

  float GetTemperature(String sensorAddress);

  void GetTemperatureAndAdress(AsyncWebServerRequest *request);
  String GetAddressToString(DeviceAddress deviceAddress);
  String GetSensorsJson();
  int GetSensorsCount();
  String GetFirstSensorAddress();
  int DeviceCount;

private:
  DallasTemperature _dallasTemperature;
  AsyncWebServer *_server;
  FS *_fs;
};
#endif