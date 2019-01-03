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
  BrewService(AsyncWebServer *server, FS *fs) : _server(server), _fs(fs) {}

  void loop();

private:
  FS *_fs;
  AsyncWebServer *_server;

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

  void LoadSettings(String settingsFile);
  void LoadBoilSettings();

  bool ExistsStepAtMoment(time_t);
};

#endif