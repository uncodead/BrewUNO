#ifndef TemperatureService_h
#define TemperatureService_h

#include <OneWire.h>
#include <DallasTemperature.h>
#include <TimeLib.h>
#include <ESPAsyncWebServer.h>
#include <ActiveStatus.h>
#include <BrewSettingsService.h>
#include <enum.h>

#define APPLICATION_JSON_TYPE "application/json"
#define GET_SENSORS_SERVICE_PATH "/rest/getsensors"

class TemperatureService
{
public:
  TemperatureService(AsyncWebServer *server, FS *fs, DallasTemperature dallasTemperature, BrewSettingsService *brewSettingsService);
  ~TemperatureService();

  BrewSettingsService *_brewSettingsService;

  Temperatures GetTemperatures();

  void GetTemperatureAndAdress(AsyncWebServerRequest *request);
  String GetAddressToString(DeviceAddress deviceAddress);
  String GetSensorsJson();
  String GetFirstSensorAddress();
  int DeviceCount;

private:
  DallasTemperature _dallasTemperature;
  AsyncWebServer *_server;
  FS *_fs;
  float getTemperature();
};
#endif