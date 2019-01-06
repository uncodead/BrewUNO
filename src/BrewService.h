#ifndef BrewService_h
#define BrewService_h

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

#define MASH_SETTINGS_FILE "/config/mashSettings.json"
#define BOIL_SETTINGS_FILE "/config/boilSettings.json"

class BrewService
{
public:
  BrewService(AsyncWebServer *server, FS *fs);

  ~BrewService();

  void loop();

private:
  AsyncWebServer *_server;
  FS *_fs;
  JsonObject *BoilSettings;

  typedef enum StepType
  {
    mash,
    boil,
    none
  };

  StepType ActiveStep;
  int ActiveStepIndex;
  float TargetTemperature;
  time_t EndTime;
  time_t StartTime;

  JsonObject &LoadSettings(String settingsFile);
  void LoadBoilSettings();

  void SetBoiIndexStep(time_t);
  String boilStepIndex;
};

#endif