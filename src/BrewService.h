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
  JsonObject *_mashSettings;

  typedef enum StepType
  {
    mash,
    boil,
    none
  };

  boolean _brewStarted;
  StepType _activeStep;

  int _activeMashStepIndex;
  String _boilStepIndex;

  float _targetTemperature;
  float _temperature;
  time_t _endTime;
  time_t _startTime;

  void loopBoil(time_t timeNow);
  void loopMash(time_t timeNow);
  void LoadBoilSettings();
  void LoadMashSettings();
  void SetBoiIndexStep(time_t);
  JsonObject &LoadSettings(String settingsFile);
  float getTemperature();

  //temp
  void temp(AsyncWebServerRequest *request);

  void getActiveStep(AsyncWebServerRequest *request);
  void startBrew(AsyncWebServerRequest *request);
};
#endif