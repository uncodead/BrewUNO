#include <MashSettingsService.h>

MashSettingsService::MashSettingsService(AsyncWebServer *server, FS *fs)
    : BrewListService(server, fs,
                      GET_MASH_SETTINGS_SERVICE_PATH,
                      POST_MASH_SETTINGS_SERVICE_PATH,
                      MASH_SETTINGS_FILE){}

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