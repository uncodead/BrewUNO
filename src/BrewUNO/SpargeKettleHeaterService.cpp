#include <BrewUNO/SpargeKettleHeaterService.h>

SpargeKettleHeaterService::SpargeKettleHeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus, BrewSettingsService *brewSettingsService, PID *kettlePID, int heaterBus) : HeaterService(temperatureService, activeStatus, brewSettingsService, kettlePID, heaterBus)
{
}

boolean SpargeKettleHeaterService::StopCompute()
{
  return !_activeStatus->BrewStarted || _activeStatus->ActiveStep != mash || _activeStatus->PWM > 100;
}