#ifndef CoolingSettingsService_h
#define CoolingSettingsService_h

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP_PLATFORM)
#include <WiFi.h>
#include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncArduinoJson6.h>
#include <IPAddress.h>
#include <SettingsService.h>
#include <BrewListService.h>
#include <BrewSettingsService.h>

#define POST_COOLING_SETTINGS_SERVICE_PATH "/rest/saveCoolingSettings"
#define GET_COOLING_SETTINGS_SERVICE_PATH "/rest/getCoolingSettings"

#define COOLING_SETTINGS_FILE "/config/coolingSettings.json"

class CoolingSettingsService : public BrewListService
{
  public:
    CoolingSettingsService(AsyncWebServer *server, FS *fs);

  protected:
    bool jsonSchemaIsValid(JsonDocument &jsonObj, String& messages);
};
#endif
