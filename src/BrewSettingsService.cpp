#include <BrewSettingsService.h>

BrewSettingsService::BrewSettingsService(AsyncWebServer *server, FS *fs)
    : SettingsService(server, fs, BREW_SETTINGS_SERVICE_PATH, BREW_SETTINGS_FILE) {}

BrewSettingsService::~BrewSettingsService() {}

void BrewSettingsService::readFromJsonObject(JsonObject &root)
{
    _boilTemperature = root["boilTemperature"];
    _boilPercent = root["boilPercent"];
    BoilTime = root["boilTime"];
    _sampleTime = root["sampleTime"];
    _kP = root["kP"];
    _kI = root["kI"];
    _kD = root["kD"];
}

void BrewSettingsService::writeToJsonObject(JsonObject &root)
{
    // connection settings
    root["boilTemperature"] = _boilTemperature;
    root["boilPercent"] = _boilPercent;
    root["boilTime"] = BoilTime;
    root["sampleTime"] = _sampleTime;
    root["kP"] = _kP;
    root["kI"] = _kI;
    root["kD"] = _kD;
}

void BrewSettingsService::onConfigUpdated()
{
}

void BrewSettingsService::begin()
{
    SettingsService::begin();
}