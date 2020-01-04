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
#include <AsyncArduinoJson6.h>
#include <IPAddress.h>
#include <AsyncJsonWebHandler.h>
#include <ActiveStatus.h>

class BrewListService
{

public:
  BrewListService(AsyncWebServer *server, FS *fs, char const *getServicePath, char const *postServicePath, String settingsFile) : _server(server), _fs(fs), _settingsFile(settingsFile)
  {
    _server->on(getServicePath, HTTP_GET, std::bind(&BrewListService::get, this, std::placeholders::_1));
    // configure update settings handler
    _updateHandler.setUri(postServicePath);
    _updateHandler.setMethod(HTTP_POST);
    _updateHandler.setMaxContentLength(MAX_ACTIVESTATUS_SIZE);
    _updateHandler.onRequest(std::bind(&BrewListService::save, this, std::placeholders::_1, std::placeholders::_2));
    _server->addHandler(&_updateHandler);
  }

protected:
  virtual bool jsonSchemaIsValid(JsonDocument &jsonObj, String &messages) {}

private:
  FS *_fs;
  AsyncWebServer *_server;
  String _settingsFile;
  AsyncJsonWebHandler _updateHandler;

  void save(AsyncWebServerRequest *request, JsonDocument &json)
  {
    if (json.is<JsonObject>())
    {
      String messages = "";
      if (!jsonSchemaIsValid(json, messages))
      {
        request->send(500, "text/plain charset=utf-8", "Error validating json: " + messages);
        return;
      }

      File configFile = _fs->open(_settingsFile, "w");
      if (!configFile)
      {
        request->send(500, "text/plain charset=utf-8", "Error at config file");
        return;
      }

      serializeJson(json, configFile);

      configFile.close();

      AsyncJsonResponse *response = new AsyncJsonResponse(MAX_ACTIVESTATUS_SIZE);
      response->setLength();
      request->send(response);
    }
    else
    {
      request->send(400, "text/plain charset=utf-8", "Invalid Json");
    }
  }

  void get(AsyncWebServerRequest *request)
  {
    File configFile = _fs->open(_settingsFile, "r");

    AsyncJsonResponse *response = new AsyncJsonResponse(MAX_ACTIVESTATUS_SIZE);
    JsonObject root = response->getRoot();

    if (configFile)
    {
      DynamicJsonDocument _jsonDocument = DynamicJsonDocument(MAX_ACTIVESTATUS_SIZE);
      DeserializationError error = deserializeJson(_jsonDocument, configFile);
      if (error == DeserializationError::Ok && _jsonDocument.is<JsonObject>())
      {
        JsonObject json = _jsonDocument.as<JsonObject>();
        root["st"] = json["st"];
        configFile.close();
      }
      else {
        root["error"] = "true";
      }

      configFile.close();
    }

    response->setLength();
    request->send(response);
  }
};

#endif
