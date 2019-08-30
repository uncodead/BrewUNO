#ifndef InternationalizationService_h
#define InternationalizationService_h

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <IPAddress.h>
#include <AsyncJsonWebHandler.h>

#include <BrewUNO/BrewSettingsService.h>

#define GET_LANG "/rest/getlang"
#define APPLICATION_JSON_TYPE "application/json"

class InternationalizationService
{
private:
  FS *_fs;
  AsyncWebServer *_server;
  BrewSettingsService *_brewSettingsService;

  void getLang(AsyncWebServerRequest *request);

public:
  InternationalizationService(AsyncWebServer *server, FS *fs, BrewSettingsService *brewSettingsServic);
  ~InternationalizationService();
};

InternationalizationService::InternationalizationService(AsyncWebServer *server, FS *fs, BrewSettingsService *brewSettingsService) : _server(server),
                                                                                                                                     _fs(fs),
                                                                                                                                     _brewSettingsService(brewSettingsService)
{
  _server->on(GET_LANG, HTTP_GET, std::bind(&InternationalizationService::getLang, this, std::placeholders::_1));
}

InternationalizationService::~InternationalizationService()
{
}

void InternationalizationService::getLang(AsyncWebServerRequest *request)
{
  File configFile = _fs->open(_brewSettingsService->Language, "r");

  AsyncJsonResponse *response = new AsyncJsonResponse(MAX_ACTIVESTATUS_SIZE);
  JsonObject root = response->getRoot();

  if (configFile)
  {
    DynamicJsonDocument _jsonDocument = DynamicJsonDocument(MAX_ACTIVESTATUS_SIZE);
    DeserializationError error = deserializeJson(_jsonDocument, configFile);
    if (error == DeserializationError::Ok && _jsonDocument.is<JsonObject>())
    {
      root["ui"] = _jsonDocument.as<JsonObject>()["ui"];
      configFile.close();
    }
    configFile.close();
  }

  response->setLength();
  request->send(response);
}

#endif