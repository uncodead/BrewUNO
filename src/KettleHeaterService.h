#ifndef KettleHeaterService_h
#define KettleHeaterService_h

#include <TemperatureService.h>
#include <ActiveStatus.h>
#include <PID_v1.h>
#include <enum.h>

class KettleHeaterService
{
public:
  KettleHeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus);
  ~KettleHeaterService();

  void SetTunings(double kp, double ki, double kd);
  void SetSampleTime(int sampleTime);
  void Compute();
  void EnablePID();
  void DisablePID();
  void RestartPID();

private:
  TemperatureService *_temperatureService;
  double _rampPowerPercentage;

  void endAutoTune();
  void cancelAutoTune();
  void startAutoTune();
  void checkHeatOff();

  ActiveStatus *_activeStatus;
};
#endif