#ifndef BoilService_h
#define BoilService_h

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
#include <WiFi.h>
#include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <IPAddress.h>
#include <AsyncJsonRequestWebHandler.h>
#include <AsyncJsonCallbackResponse.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <enum.h>
#include <TemperatureService.h>
#include <ActiveStatus.h>

#define BOIL_SETTINGS_FILE "/config/boilSettings.json"

class BoilService
{
public:
  BoilService(FS *fs, TemperatureService *temperatureService);

  ~BoilService();

  void loop(ActiveStatus *activeStatus);
  void LoadBoilSettings();

private:
  FS *_fs;
  JsonObject *_boilSettings;
  TemperatureService *_temperatureService;

  void SetBoiIndexStep(ActiveStatus *activeStatus, time_t);
  JsonObject &LoadSettings(String settingsFile);
};
#endif