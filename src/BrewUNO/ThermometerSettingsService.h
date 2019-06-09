#ifndef ThermometerSettingsService_h
#define ThermometerSettingsService_h

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
#include <BrewUNO/BrewListService.h>
#include <BrewUNO/BrewSettingsService.h>

#define POST_THERMOMETERS_SETTINGS_SERVICE_PATH "/rest/saveThermometers"
#define GET_THERMOMETERS_SETTINGS_SERVICE_PATH "/rest/getThermometers"

#define THERMOMETERS_SETTINGS_FILE "/config/thermometersSettings.json"

class ThermometerSettingsService : public BrewListService
{
  public:
    ThermometerSettingsService(AsyncWebServer *server, FS *fs, BrewSettingsService *brewSettings);

  protected:
    bool jsonSchemaIsValid(JsonDocument &jsonObj, String& messages);
    BrewSettingsService *_brewSettings;
};
#endif
