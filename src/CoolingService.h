#ifndef CoolingService_h
#define CoolingService_h

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
#include <WiFi.h>
#include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <AsyncArduinoJson6.h>
#include <IPAddress.h>
#include <AsyncJsonWebHandler.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <enum.h>
#include <TemperatureService.h>
#include <ActiveStatus.h>
#include <Buzzer.h>
#include <Pump.h>

#define COOLING_SETTINGS_FILE "/config/coolingSettings.json"

class CoolingService
{
public:
  CoolingService(FS *fs, TemperatureService *temperatureService);
  ~CoolingService();

  void loop(ActiveStatus *activeStatus);
  void LoadCoolingSettings();

private:
  FS *_fs;
  TemperatureService *_temperatureService;
  boolean checkTemperaturePID(ActiveStatus *activeStatus);
  void StepStarted(ActiveStatus *activeStatus, JsonArray steps, time_t timeNow);
  void NextStep(ActiveStatus *activeStatus, JsonArray steps, time_t timeNow, int nextMashStep);
  String getCoolingName(ActiveStatus *activeStatus, JsonObject step);
};
#endif