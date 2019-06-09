#include <BrewUNO/ThermometerSettingsService.h>

ThermometerSettingsService::ThermometerSettingsService(AsyncWebServer *server, FS *fs, BrewSettingsService *brewSettings)
    : _brewSettings(brewSettings), BrewListService(server, fs,
                                                   GET_THERMOMETERS_SETTINGS_SERVICE_PATH,
                                                   POST_THERMOMETERS_SETTINGS_SERVICE_PATH,
                                                   THERMOMETERS_SETTINGS_FILE) {}

bool ThermometerSettingsService::jsonSchemaIsValid(JsonDocument &jsonObj, String &messages)
{
    return true;
}