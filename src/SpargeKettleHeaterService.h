#ifndef SpargeKettleHeaterService_h
#define SpargeKettleHeaterService_h

#include <HeaterService.h>

class SpargeKettleHeaterService : public HeaterService
{
public:
  SpargeKettleHeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus, BrewSettingsService *brewSettingsService);
  void StartPID(double kp, double ki, double kd);

protected:
  void SetUP();
  boolean StopCompute();
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