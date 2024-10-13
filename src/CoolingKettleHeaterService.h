#ifndef CoolingKettleHeaterService_h
#define CoolingKettleHeaterService_h

#include <HeaterService.h>

class CoolingKettleHeaterService : public HeaterService
{
public:
  CoolingKettleHeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus, BrewSettingsService *brewSettingsService);
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
  void TurnOff();
  bool InvertedPWM();
  void SetPidParameters(double input, double setpoint);
};
#endif