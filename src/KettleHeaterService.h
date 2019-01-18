#ifndef KettleHeaterService_h
#define KettleHeaterService_h

#include <TemperatureService.h>
#include <PID_v1.h>

class KettleHeaterService
{
public:
  KettleHeaterService(TemperatureService *temperatureService);
  ~KettleHeaterService();

  void SetK(double kp, double ki, double kd);
  void SetBoilPercent(double percent);
  void SetSetpoint(double setpoint);
  void Compute(boolean);

private:
  TemperatureService *_temperatureService;
  double _boilPercent;
};
#endif