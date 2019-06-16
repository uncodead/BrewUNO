#ifndef TemperatureService_h
#define TemperatureService_h

#include <OneWire.h>
#include <DallasTemperature.h>
#include <TimeLib.h>
#include <ESPAsyncWebServer.h>
#include <BrewUNO/ActiveStatus.h>

#define APPLICATION_JSON_TYPE "application/json"
#define GET_SENSORS_SERVICE_PATH "/rest/getsensors"

struct Temperatures
{
  float Main;
  float Sparge;
  String Json;
};

class TemperatureService
{
public:
  TemperatureService(AsyncWebServer *server, FS *fs, DallasTemperature dallasTemperature);
  ~TemperatureService();

  Temperatures GetTemperatures(String main,String sparge);

  void GetTemperatureAndAdress(AsyncWebServerRequest *request);
  String GetAddressToString(DeviceAddress deviceAddress);
  String GetSensorsJson();
  String GetFirstSensorAddress();
  int DeviceCount;

private:
  DallasTemperature _dallasTemperature;
  AsyncWebServer *_server;
  FS *_fs;
};
#endif