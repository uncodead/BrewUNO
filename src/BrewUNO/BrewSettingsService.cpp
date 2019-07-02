#include <BrewUNO/BrewSettingsService.h>

BrewSettingsService::BrewSettingsService(AsyncWebServer *server, FS *fs, ActiveStatus *activeStatus)
    : _activeStatus(activeStatus), SettingsService(server, fs, BREW_SETTINGS_SERVICE_PATH, BREW_SETTINGS_FILE) {}

BrewSettingsService::~BrewSettingsService() {}

void BrewSettingsService::readFromJsonObject(JsonObject &root)
{
    BoilTemperature = root["boilTemperature"];
    BoilPowerPercentage = root["boilPowerPercentage"];
    SpargePowerPercentage = root["spargePowerPercentage"];
    EnableSparge = root["enableSparge"];
    BoilTime = root["boilTime"];
    KP = root["kP"];
    KI = root["kI"];
    KD = root["kD"];
    PumpRestInterval = root["pumpRestInterval"];
    PumpRestTime = root["pumpRestTime"];
    PIDStart = root["pidStart"];
    MashHeaterPercentage = root["mashHeaterPercentage"];
    MainSensor = root["mainSensor"] | "";
    SpargeSensor = root["spargeSensor"] | "";
}

void BrewSettingsService::writeToJsonObject(JsonObject &root)
{
    root["boilTemperature"] = BoilTemperature;
    root["boilPowerPercentage"] = BoilPowerPercentage;
    root["spargePowerPercentage"] = SpargePowerPercentage;
    root["enableSparge"] = EnableSparge;
    root["boilTime"] = BoilTime;
    root["kP"] = KP;
    root["kI"] = KI;
    root["kD"] = KD;
    root["pumpRestInterval"] = PumpRestInterval;
    root["pumpRestTime"] = PumpRestTime;
    root["pidStart"] = PIDStart;
    root["mashHeaterPercentage"] = MashHeaterPercentage;
    root["mainSensor"] = MainSensor;
    root["spargeSensor"] = SpargeSensor;
    _activeStatus->PIDSettingsUpdated = true;
}

void BrewSettingsService::onConfigUpdated()
{
}

void BrewSettingsService::begin()
{
    SettingsService::begin();
}