#include <BrewSettingsService.h>

BrewSettingsService::BrewSettingsService(AsyncWebServer *server, FS *fs)
    : SettingsService(server, fs, BREW_SETTINGS_SERVICE_PATH, BREW_SETTINGS_FILE) {}

BrewSettingsService::~BrewSettingsService() {}

void BrewSettingsService::readFromJsonObject(JsonObject &root)
{
    _boilPercent = root["boilPercent"];
    _boilTime = root["boilTime"];
    _sampleTime = root["sampleTime"];
    _kP = root["kP"];
    _kI = root["kI"];
    _kD = root["kD"];
}

void BrewSettingsService::writeToJsonObject(JsonObject &root)
{
    // connection settings
    root["boilPercent"] = _boilPercent;
    root["boilTime"] = _boilTime;
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