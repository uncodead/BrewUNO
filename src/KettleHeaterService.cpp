#include <KettleHeaterService.h>

#define HEATER_BUS D7

double KettleSetpoint, KettleInput, KettleOutput;
PID kettlePID = PID(&KettleInput, &KettleOutput, &KettleSetpoint, 2, 5, 1, DIRECT);

KettleHeaterService::KettleHeaterService(TemperatureService *temperatureService) : _temperatureService(temperatureService)
{
}

KettleHeaterService::~KettleHeaterService() {}

void KettleHeaterService::SetTunings(double kp, double ki, double kd, int sampleTime)
{
  kettlePID.SetMode(MANUAL);
  kettlePID.SetTunings(kp, ki, kd);
  kettlePID.SetSampleTime(sampleTime);
}

void KettleHeaterService::RestartPID()
{
  kettlePID = PID(&KettleInput, &KettleOutput, &KettleSetpoint, 2, 5, 1, DIRECT);
}

void KettleHeaterService::SetMode(int mode)
{
  kettlePID.SetMode(mode);
}

void KettleHeaterService::SetBoilPercent(double percent)
{
  _boilPercent = percent;
}

void KettleHeaterService::Compute(ActiveStatus *activeStatus)
{
  if (!activeStatus->BrewStarted || activeStatus->ActiveStep == none)
  {
    kettlePID.SetMode(MANUAL);
    analogWrite(HEATER_BUS, 0);
    return;
  }

  kettlePID.SetMode(AUTOMATIC);
  kettlePID.SetOutputLimits(0, 1023);
  KettleSetpoint = activeStatus->TargetTemperature;

  KettleInput = _temperatureService->GetTemperature();
  activeStatus->Temperature = KettleInput;
  activeStatus->LogTemperature(KettleInput, KettleSetpoint);

  kettlePID.Compute();

  if (activeStatus->ActiveStep == boil && activeStatus->StartTime > 0)
  {
    KettleOutput = (KettleOutput * _boilPercent) / 100;
  }

  activeStatus->PWM = KettleOutput;
  Serial.println(KettleOutput);

  analogWrite(HEATER_BUS, KettleOutput);
}