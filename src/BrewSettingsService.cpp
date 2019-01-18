#include <BrewSettingsService.h>

BrewSettingsService::BrewSettingsService(AsyncWebServer *server, FS *fs)
    : SettingsService(server, fs, BREW_SETTINGS_SERVICE_PATH, BREW_SETTINGS_FILE) {}

BrewSettingsService::~BrewSettingsService() {}

void BrewSettingsService::readFromJsonObject(JsonObject &root)
{
    BoilTemperature = root["boilTemperature"];
    BoilPercent = root["boilPercent"];
    BoilTime = root["boilTime"];
    SampleTime = root["sampleTime"];
    KP = root["kP"];
    KI = root["kI"];
    KD = root["kD"];
}

void BrewSettingsService::writeToJsonObject(JsonObject &root)
{
    // connection settings
    root["boilTemperature"] = BoilTemperature;
    root["boilPercent"] = BoilPercent;
    root["boilTime"] = BoilTime;
    root["sampleTime"] = SampleTime;
    root["kP"] = KP;
    root["kI"] = KI;
    root["kD"] = KD;
}

void BrewSettingsService::onConfigUpdated()
{
}

void BrewSettingsService::begin()
{
    SettingsService::begin();
}