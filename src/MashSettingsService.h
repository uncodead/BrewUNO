#ifndef MashSettingsService_h
#define MashSettingsService_h

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
#include <BrewListService.h>

#define POST_MASH_SETTINGS_SERVICE_PATH "/rest/saveMashSettings"
#define GET_MASH_SETTINGS_SERVICE_PATH "/rest/getMashSettings"

#define MASH_SETTINGS_FILE "/config/mashSettings.json"

class MashSettingsService : public BrewListService
{
public:
  MashSettingsService(AsyncWebServer *server, FS *fs);

protected:
  bool jsonSchemaIsValid(JsonObject &jsonObj, String& messages);
};

#endif
