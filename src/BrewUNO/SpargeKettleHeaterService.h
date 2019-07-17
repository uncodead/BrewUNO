#ifndef SpargeKettleHeaterService_h
#define SpargeKettleHeaterService_h

#include <BrewUNO/HeaterService.h>

class SpargeKettleHeaterService : public HeaterService
{
public:
  SpargeKettleHeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus, BrewSettingsService *brewSettingsService, int heaterBus);
  void StartPID(double kp, double ki, double kd);

protected:
  boolean StopCompute();
  void PidCompute();
  double GetPidOutput();
  double GetPidInput();
  double GetPidSetPoint();
  void SetPidParameters(double input, double setpoint);
};
#endif