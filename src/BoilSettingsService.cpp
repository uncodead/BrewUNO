#include <BoilSettingsService.h>

BoilSettingsService::BoilSettingsService(AsyncWebServer *server, FS *fs)
    : BrewListService(server, fs,
                      GET_BOIL_SETTINGS_SERVICE_PATH,
                      POST_BOIL_SETTINGS_SERVICE_PATH,
                      BOIL_SETTINGS_FILE) {}

bool BoilSettingsService::jsonSchemaIsValid(JsonObject &jsonObj)
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