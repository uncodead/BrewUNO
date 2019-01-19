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

#define BOIL_SETTINGS_FILE "/config/boilSettings.json"

class BoilService
{
public:
  BoilService(FS *fs, TemperatureService *temperatureService);

  ~BoilService();

  void loop(time_t timeNow, boolean &_brewStarted, StepType &_activeStep, float &_setPoint);
  void SetTemperature(float temperature);
  void SetTime(int time);
  void LoadBoilSettings();
  String GetBoilStepIndex();

private:
  FS *_fs;
  JsonObject *_boilSettings;
  TemperatureService *_temperatureService;

  String _boilStepIndex;

  float _targetTemperature;
  time_t _endTime;
  time_t _startTime;
  int _boilTime;

  void SetBoiIndexStep(time_t);
  JsonObject &LoadSettings(String settingsFile);
  float getTemperature();
};
#endif