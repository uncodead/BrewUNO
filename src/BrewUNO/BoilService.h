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
#include <AsyncArduinoJson6.h>
#include <IPAddress.h>
#include <AsyncJsonRequestWebHandler.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <BrewUNO/enum.h>
#include <BrewUNO/TemperatureService.h>
#include <BrewUNO/ActiveStatus.h>
#include <BrewUNO/Buzzer.h>

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
  JsonDocument *_boilSettings;
  TemperatureService *_temperatureService;

  void SetBoiIndexStep(ActiveStatus *activeStatus, time_t);
  JsonDocument &LoadSettings(String settingsFile);
};
#endif