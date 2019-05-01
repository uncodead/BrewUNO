#ifndef KettleHeaterService_h
#define KettleHeaterService_h

#include <BrewUNO/TemperatureService.h>
#include <BrewUNO/ActiveStatus.h>
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
#include <BrewUNO/enum.h>
#include <BrewUNO/BrewSettingsService.h>

class KettleHeaterService
{
public:
  KettleHeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus, BrewSettingsService *brewSettingsService);
  ~KettleHeaterService();

  void StartPID(double kp, double ki, double kd);
  void StartAutoTune();
  void Compute();

private:
  TemperatureService *_temperatureService;
  ActiveStatus *_activeStatus;
  BrewSettingsService *_brewSettingsService;
};
#endif