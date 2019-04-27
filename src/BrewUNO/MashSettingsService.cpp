#include <BrewUNO/MashSettingsService.h>

MashSettingsService::MashSettingsService(AsyncWebServer *server, FS *fs)
    : BrewListService(server, fs,
                      GET_MASH_SETTINGS_SERVICE_PATH,
                      POST_MASH_SETTINGS_SERVICE_PATH,
                      MASH_SETTINGS_FILE) {}

bool MashSettingsService::jsonSchemaIsValid(JsonDocument jsonObj, String &messages)
{
    JsonArray steps = jsonObj["steps"].as<JsonArray>();;
    if (steps.size() <= 0)
    {
        return false;
    }

    bool validJson = true;
    for (int i = 0; i < steps.size(); i++)
    {
        JsonObject step = steps[i];
        if (step["name"] == "")
        {
            validJson = false;
            messages += "Name could not be null. ";
        }
        if (step["temperature"] <= 0)
        {
            validJson = false;
            messages += "Temperature could not be zero. ";
        }
        if (step["time"] <= 0)
        {
            validJson = false;
            messages += "Time could not be zero.";
        };
    }
    return validJson;
}