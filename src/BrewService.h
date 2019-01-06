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
  JsonObject *_boilSettings;

  typedef enum StepType
  {
    mash,
    boil,
    none
  };

  StepType _activeStep;
  int _activeMashStepIndex;
  float _targetTemperature;
  time_t _endTime;
  time_t _startTime;

  String _boilStepIndex;

  void loopBoil(time_t timeNow);
  void loopMash(time_t timeNow);
  void LoadBoilSettings();
  void SetBoiIndexStep(time_t);
  JsonObject &LoadSettings(String settingsFile);
};

#endif