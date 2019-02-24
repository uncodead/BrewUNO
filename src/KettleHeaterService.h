#ifndef KettleHeaterService_h
#define KettleHeaterService_h

#include <TemperatureService.h>
#include <ActiveStatus.h>
#include <PID_v1.h>
#include <enum.h>

class KettleHeaterService
{
public:
  KettleHeaterService(TemperatureService *temperatureService);
  ~KettleHeaterService();

  void SetTunings(double kp, double ki, double kd, int sampleTime);
  void SetBoilPercent(double percent);
  void Compute(ActiveStatus *activeStatus);
  void SetMode(int mode);
  void RestartPID();

private:
  TemperatureService *_temperatureService;
  double _boilPercent;
};
#endif