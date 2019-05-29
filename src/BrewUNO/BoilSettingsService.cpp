#include <BrewUNO/BoilSettingsService.h>

BoilSettingsService::BoilSettingsService(AsyncWebServer *server, FS *fs, BrewSettingsService *brewSettings)
    : _brewSettings(brewSettings), BrewListService(server, fs,
                                                   GET_BOIL_SETTINGS_SERVICE_PATH,
                                                   POST_BOIL_SETTINGS_SERVICE_PATH,
                                                   BOIL_SETTINGS_FILE) {}

bool BoilSettingsService::jsonSchemaIsValid(JsonDocument &jsonObj, String &messages)
{
    JsonArray steps = jsonObj["st"];

    bool validJson = true;
    for (int i = 0; i < steps.size(); i++)
    {
        JsonObject step = steps.getElement(i);
        if (step["n"] == "")
        {
            validJson = false;
            messages += "Name could not be null. ";
        }
        if (step["a"] <= 0)
        {
            validJson = false;
            messages += " - Amount could not be zero. ";
        }
        if (step["tm"] > _brewSettings->BoilTime)
        {
            validJson = false;
            messages += " - Time exceeded the setting for boiling, check settings.";
        }
    }
    return validJson;
}