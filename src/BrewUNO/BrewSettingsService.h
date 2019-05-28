#ifndef BrewSettingsService_h
#define BrewSettingsService_h

#include <SettingsService.h>
#include <IPAddress.h>

#define BREW_SETTINGS_FILE "/config/brewSettings.json"
#define BREW_SETTINGS_SERVICE_PATH "/rest/brewSettings"

class BrewSettingsService : public SettingsService
{

public:
  BrewSettingsService(AsyncWebServer *server, FS *fs);
  ~BrewSettingsService();

  void begin();

  double KP;
  double KI;
  double KD;

  int BoilTime;
  double RampPowerPercentage;
  double BoilPowerPercentage;
  double BoilTemperature;
  double PIDStart;
  double MashHeaterPercentage;
  int SampleTime;

  int PumpRestInterval;
  int PumpRestTime;

protected:
  void readFromJsonObject(JsonObject &root);
  void writeToJsonObject(JsonObject &root);
  void onConfigUpdated();
};

#endif
