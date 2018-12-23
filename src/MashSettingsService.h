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

#define SAVE_MASH_SETUP_SERVICE_PATH "/rest/saveMashSettings"

class MashSettingsService {

  public:

    MashSettingsService(AsyncWebServer *server);

  private:

    AsyncWebServer* _server;

    void save(AsyncWebServerRequest *request);

};

#endif 
