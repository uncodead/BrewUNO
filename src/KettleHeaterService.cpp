#include <KettleHeaterService.h>

#define HEATER_BUS D3

double KettleSetpoint, KettleInput, KettleOutput;
PID kettlePID = PID(&KettleInput, &KettleOutput, &KettleSetpoint, 2, 5, 1, DIRECT);

KettleHeaterService::KettleHeaterService(TemperatureService *temperatureService) : _temperatureService(temperatureService)
{
}

KettleHeaterService::~KettleHeaterService() {}

void KettleHeaterService::SetTunings(double kp, double ki, double kd)
{
  kettlePID.SetTunings(kp, ki, kd);
}

void KettleHeaterService::SetBoilPercent(double percent)
{
  _boilPercent = percent;
}

void KettleHeaterService::Compute(ActiveStatus *activeStatus)
{
  if (activeStatus->ActiveStep == none)
  {
    kettlePID.SetMode(MANUAL);
    analogWrite(HEATER_BUS, 0);
    return;
  }

  kettlePID.SetMode(AUTOMATIC);
  KettleSetpoint = activeStatus->TargetTemperature;

  KettleInput = _temperatureService->GetTemperature();
  kettlePID.Compute();

  if (KettleOutput < 0)
  {
    KettleOutput = 0;
  }
  if (KettleOutput > 1023)
  {
    KettleOutput = 1023;
  }

  if (activeStatus->ActiveStep == boil)
  {
    KettleOutput = (KettleOutput * _boilPercent) / 100;
  }

  Serial.println(KettleOutput);

  analogWrite(HEATER_BUS, KettleOutput);
}