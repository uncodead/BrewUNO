#ifndef SpargeKettleHeaterService_h
#define SpargeKettleHeaterService_h

#include <BrewUNO/HeaterService.h>

class SpargeKettleHeaterService : public HeaterService
{
public:
  SpargeKettleHeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus, BrewSettingsService *brewSettingsService, PID *kettlePID, int heaterBus);

protected:
  boolean StopCompute();
};
#endif