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
#include <AsyncArduinoJson6.h>
#include <IPAddress.h>
#include <AsyncJsonWebHandler.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <BrewUNO/enum.h>
#include <BrewUNO/TemperatureService.h>
#include <BrewUNO/ActiveStatus.h>
#include <BrewUNO/Buzzer.h>
#include <BrewUNO/Pump.h>

#define MASH_SETTINGS_FILE "/config/mashSettings.json"

class MashService
{
public:
  MashService(FS *fs, TemperatureService *temperatureService, Pump *pump);
  ~MashService();

  void loop(ActiveStatus *activeStatus);
  void LoadMashSettings();

private:
  FS *_fs;
  TemperatureService *_temperatureService;
  Pump *_pump;
  boolean checkTemperaturePID(ActiveStatus *activeStatus);
  String getMashName(JsonObject step);
};
#endif