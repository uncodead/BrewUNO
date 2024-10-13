#include <CoolingSettingsService.h>

CoolingSettingsService::CoolingSettingsService(AsyncWebServer *server, FS *fs)
    : BrewListService(server, fs,
                      GET_COOLING_SETTINGS_SERVICE_PATH,
                      POST_COOLING_SETTINGS_SERVICE_PATH,
                      COOLING_SETTINGS_FILE) {}

bool CoolingSettingsService::jsonSchemaIsValid(JsonDocument &jsonObj, String &messages)
{
    JsonArray steps = jsonObj["st"];

    bool validJson = true;
    for (int i = 0; i < steps.size(); i++)
    {
        JsonObject step = steps[i];
        if (step["n"] == "")
        {
            validJson = false;
            messages += "Name could not be null. ";
        }
        if (step["t"] <= 0)
        {
            validJson = false;
            messages += "Temperature could not be zero. ";
        }
        if (step["tm"] <= 0)
        {
            validJson = false;
            messages += "Time could not be zero.";
        };
    }
    return validJson;
}