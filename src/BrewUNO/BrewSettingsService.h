#ifndef BrewSettingsService_h
#define BrewSettingsService_h

#include <SettingsService.h>
#include <IPAddress.h>
#include <BrewUNO/ActiveStatus.h>

#define BREW_SETTINGS_FILE "/config/brewSettings.json"
#define BREW_SETTINGS_SERVICE_PATH "/rest/brewSettings"

class BrewSettingsService : public SettingsService
{

public:
  BrewSettingsService(AsyncWebServer *server, FS *fs, ActiveStatus *activeStatus);
  ~BrewSettingsService();

  void begin();

  double KP;
  double KI;
  double KD;

  int BoilTime;
  double RampPowerPercentage;
  double BoilPowerPercentage;
  double SpargePowerPercentage;
  double SpargeTemperature;
  bool EnableSparge;
  double BoilTemperature;
  double PIDStart;
  double MashHeaterPercentage;

  int PumpRestInterval;
  int PumpRestTime;

  double MainSensorOffset;
  double SpargeSensorOffset;
  double AuxSensorOneOffset;
  double AuxSensorTwoOffset;
  double AuxSensorThreeOffset;

  String MainSensor;
  String SpargeSensor;
  String AuxOneSensor;
  String AuxTwoSensor;
  String AuxThreeSensor;
  String Language;
  String TempUnit;

protected:
  void readFromJsonObject(JsonObject &root);
  void writeToJsonObject(JsonObject &root);
  void onConfigUpdated();

private:
  ActiveStatus *_activeStatus;
};

#endif
