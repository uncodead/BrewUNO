#ifndef MashKettleHeaterService_h
#define MashKettleHeaterService_h

#include <BrewUNO/HeaterService.h>

class MashKettleHeaterService : public HeaterService
{
public:
  MashKettleHeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus, BrewSettingsService *brewSettingsService, PID *kettlePID, int heaterBus);

protected:
  boolean StopCompute();
};
#endif