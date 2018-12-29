#ifndef BrewListService_h
#define BrewListService_h

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

class BrewListService
{

public:
  BrewListService(AsyncWebServer *server, FS *fs, char const *getServicePath, char const *postServicePath, String settingsFile) : _server(server), _fs(fs), _settingsFile(settingsFile)
  {
    _server->on(getServicePath, HTTP_GET, std::bind(&BrewListService::get, this, std::placeholders::_1));
    _server->addHandler(new AsyncCallbackJsonWebHandler(postServicePath, std::bind(&BrewListService::save, this, std::placeholders::_1, std::placeholders::_2)));
  }

protected:
  virtual bool jsonSchemaIsValid(JsonObject &jsonObj) {}

private:
  FS *_fs;
  AsyncWebServer *_server;
  String _settingsFile;

  void save(AsyncWebServerRequest *request, JsonVariant &json)
  {
    JsonObject &jsonObj = json.as<JsonObject>();
    if (jsonObj.success() && jsonSchemaIsValid(jsonObj))
    {
      File configFile = _fs->open(_settingsFile, "w");
      if (!configFile)
      {
        request->send(500, "Error at config file");
        return;
      }
      jsonObj.printTo(configFile);
      configFile.close();

      AsyncJsonResponse *response = new AsyncJsonResponse();
      response->setLength();
      request->send(response);
    }
    else
    {
      request->send(400);
    }
  }

  void get(AsyncWebServerRequest *request)
  {
    AsyncJsonResponse *response = new AsyncJsonResponse();

    File configFile = _fs->open(_settingsFile, "r");
    DynamicJsonBuffer jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(configFile);
    configFile.close();

    JsonObject &jsonResponse = response->getRoot();
    jsonResponse["steps"] = root["steps"];
    response->setLength();
    request->send(response);
  }
};

#endif
