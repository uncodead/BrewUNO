#ifndef MashKettleHeaterService_h
#define MashKettleHeaterService_h

#include <BrewUNO/HeaterService.h>

class MashKettleHeaterService : public HeaterService
{
public:
  MashKettleHeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus, BrewSettingsService *brewSettingsService);
  void StartPID(double kp, double ki, double kd);

protected:
  void SetUP();
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