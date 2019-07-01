#include <BrewUNO/MashKettleHeaterService.h>

MashKettleHeaterService::MashKettleHeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus, BrewSettingsService *brewSettingsService, PID *kettlePID, int heaterBus) : HeaterService(temperatureService, activeStatus, brewSettingsService, kettlePID, heaterBus)
{
}

boolean MashKettleHeaterService::StopCompute()
{
  return !_activeStatus->BrewStarted || _activeStatus->ActiveStep == none || _activeStatus->PumpIsResting || _activeStatus->HeaterOff;
}