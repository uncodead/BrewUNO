#ifndef BoilKettleHeaterService_h
#define BoilKettleHeaterService_h

#include <BrewUNO/HeaterService.h>

class BoilKettleHeaterService : public HeaterService
{
public:
  BoilKettleHeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus, BrewSettingsService *brewSettingsService);
  void StartPID(double kp, double ki, double kd);

protected:
  boolean StopCompute();
  PID &GetPid();
  void PidCompute();
  double GetPidOutput();
  double GetPidInput();
  double GetPidSetPoint();
  uint8_t GetBus();
  void SetPidParameters(double input, double setpoint);
};
#endif