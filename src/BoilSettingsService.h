#ifndef BoilSettingsService_h
#define BoilSettingsService_h

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
#include <SettingsService.h>
#include <BrewListService.h>
#include <BrewSettingsService.h>

#define POST_BOIL_SETTINGS_SERVICE_PATH "/rest/saveBoilSettings"
#define GET_BOIL_SETTINGS_SERVICE_PATH "/rest/getBoilSettings"

#define BOIL_SETTINGS_FILE "/config/boilSettings.json"

class BoilSettingsService : public BrewListService
{
  public:
    BoilSettingsService(AsyncWebServer *server, FS *fs, BrewSettingsService *brewSettings);

  protected:
    bool jsonSchemaIsValid(JsonObject &jsonObj, String& messages);
    BrewSettingsService *_brewSettings;
};
#endif
