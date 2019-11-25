#include <BrewSettingsService.h>

BrewSettingsService::BrewSettingsService(AsyncWebServer *server, FS *fs, ActiveStatus *activeStatus) : _activeStatus(activeStatus),
                                                                                                       SettingsService(server, fs, BREW_SETTINGS_SERVICE_PATH, BREW_SETTINGS_FILE) {}

BrewSettingsService::~BrewSettingsService() {}

void BrewSettingsService::readFromJsonObject(JsonObject &root)
{
    BoilTemperature = root["btemp"];
    BoilPowerPercentage = root["bpp"];
    SpargePowerPercentage = root["spp"];
    SpargeTemperature = root["st"];
    EnableSparge = root["es"];
    EnableBoilKettle = root["ebk"];
    BoilTime = root["bt"];
    KP = root["kP"];
    KI = root["kI"];
    KD = root["kD"];
    PumpRestInterval = root["pri"];
    PumpRestTime = root["prt"];
    PIDStart = root["ps"];
    MashHeaterPercentage = root["mhp"];
    MainSensor = root["ms"] | "";
    SpargeSensor = root["ss"] | "";
    BoilSensor = root["bs"] | "";
    AuxOneSensor = root["aso"] | "";
    AuxTwoSensor = root["ast"] | "";
    AuxThreeSensor = root["asth"] | "";
    AuxSensorOneOffset = root["asoo"];
    AuxSensorTwoOffset = root["asto"];
    AuxSensorThreeOffset = root["astho"];
    MainSensorOffset = root["mso"];
    SpargeSensorOffset = root["sso"];
    BoilSensorOffset = root["bso"];
    Language = root["lg"] | "";
    TempUnit = root["tu"] | "";
}

void BrewSettingsService::writeToJsonObject(JsonObject &root)
{
    root["btemp"] = BoilTemperature;
    root["bpp"] = BoilPowerPercentage;
    root["spp"] = SpargePowerPercentage;
    root["es"] = EnableSparge;
    root["ebk"] = EnableBoilKettle;
    root["st"] = SpargeTemperature;
    root["bt"] = BoilTime;
    root["kP"] = KP;
    root["kI"] = KI;
    root["kD"] = KD;
    root["pri"] = PumpRestInterval;
    root["prt"] = PumpRestTime;
    root["ps"] = PIDStart;
    root["mhp"] = MashHeaterPercentage;
    root["ms"] = MainSensor;
    root["ss"] = SpargeSensor;
    root["bs"] = BoilSensor;
    root["mso"] = MainSensorOffset;
    root["sso"] = SpargeSensorOffset;
    root["bso"] = BoilSensorOffset;
    root["lg"] = Language;
    root["tu"] = TempUnit;
    root["aso"] = AuxOneSensor;
    root["ast"] = AuxTwoSensor;
    root["asth"] = AuxThreeSensor;
    root["asoo"] = AuxSensorOneOffset;
    root["asto"] = AuxSensorTwoOffset;
    root["astho"] = AuxSensorThreeOffset;
    _activeStatus->PIDSettingsUpdated = true;
}

void BrewSettingsService::onConfigUpdated()
{
}

void BrewSettingsService::begin()
{
    SettingsService::begin();
}