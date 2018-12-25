#include <MashSettingsService.h>

MashSettingsService::MashSettingsService(AsyncWebServer *server, FS *fs) : _server(server), _fs(fs)
{
    _server->on(GET_MASH_SETTINGS_SERVICE_PATH, HTTP_GET, std::bind(&MashSettingsService::get, this, std::placeholders::_1));
    _server->addHandler(new AsyncCallbackJsonWebHandler(POST_MASH_SETTINGS_SERVICE_PATH, std::bind(&MashSettingsService::save, this, std::placeholders::_1, std::placeholders::_2)));
}

void MashSettingsService::save(AsyncWebServerRequest *request, JsonVariant &json)
{
    JsonObject &jsonObj = json.as<JsonObject>();
    if (jsonObj.success() && jsonSchemaIsValid(jsonObj))
    {
        File configFile = _fs->open(MASH_SETTINGS_FILE, "w");
        if (!configFile)
        {
            request->send(400);
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

bool MashSettingsService::jsonSchemaIsValid(JsonObject &jsonObj)
{
    JsonArray &steps = jsonObj["steps"];
    if (steps.size() <= 0)
    {
        return false;
    }

    bool validJson = true;
    for (int i = 0; i < steps.size(); i++)
    {
        JsonObject &step = steps[i];
        validJson = validJson &&
                    step["name"] != "" &&
                    step["temperature"] > 0 &&
                    step["time"] > 0;
    }
    return validJson;
}

void MashSettingsService::get(AsyncWebServerRequest *request)
{
    AsyncJsonResponse *response = new AsyncJsonResponse();

    File configFile = _fs->open(MASH_SETTINGS_FILE, "r");
    DynamicJsonBuffer jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(configFile);
    configFile.close();

    JsonObject &jsonResponse = response->getRoot();
    jsonResponse["steps"] = root["steps"];
    response->setLength();
    request->send(response);
}