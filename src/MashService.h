#ifndef MashService_h
#define MashService_h

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

#define MASH_SETTINGS_FILE "/config/mashSettings.json"

class MashService
{
public:
  MashService(FS *fs, TemperatureService *temperatureService);
  ~MashService();

  void loop(time_t timeNow, boolean &_brewStarted, StepType &_activeStep, float &_setPoint);
  void LoadMashSettings();
  int GetActiveStep();

private:
  FS *_fs;
  JsonObject *_mashSettings;
  TemperatureService *_temperatureService;

  int _activeMashStepIndex;
  
  float _targetTemperature;
  time_t _endTime;
  time_t _startTime;

  JsonObject &LoadSettings(String settingsFile);
  float getTemperature();
};
#endif