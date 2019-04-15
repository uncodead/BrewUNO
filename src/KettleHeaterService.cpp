#include <KettleHeaterService.h>

boolean _heatOff = true;
double KettleSetpoint, KettleInput, KettleOutput;
PID _kettlePID = PID(&KettleInput, &KettleOutput, &KettleSetpoint, 1, 1, 1, DIRECT);

KettleHeaterService::KettleHeaterService(TemperatureService *temperatureService, ActiveStatus *activeStatus) : _temperatureService(temperatureService),
                                                                                                               _activeStatus(activeStatus)
{
}

KettleHeaterService::~KettleHeaterService() {}

void KettleHeaterService::SetSampleTime(int sampleTime)
{
  _kettlePID.SetSampleTime(sampleTime);
  _kettlePID.SetOutputLimits(0, 1023);
}

void KettleHeaterService::SetTunings(double kp, double ki, double kd)
{
  _heatOff = true;
  _kettlePID.SetTunings(kp, ki, kd);
}

void KettleHeaterService::StopPID()
{
  _kettlePID.SetMode(AUTOMATIC);

  _kettlePID.SetOutputLimits(0.0, 1.0);
  _kettlePID.SetOutputLimits(-1.0, 0.0);
  _kettlePID.SetOutputLimits(0, 1023);

  _kettlePID.SetMode(MANUAL);
}

void KettleHeaterService::DisablePID()
{
  StopPID();
  _heatOff = true;
  analogWrite(HEATER_BUS, 0);
}

void KettleHeaterService::EnablePID()
{
  _kettlePID.SetMode(AUTOMATIC);
}

void KettleHeaterService::checkHeatOff()
{
  if (_heatOff)
  {
    Serial.print("Heat Off, turn on... ");
    EnablePID();
    _heatOff = false;
  }
}

void KettleHeaterService::generatePWM()
{
  KettleInput = _activeStatus->Temperature;
  KettleSetpoint = _activeStatus->TargetTemperature;

  if (_activeStatus->ActiveStep == mash)
  {
    if ((KettleSetpoint - KettleInput) < 3)
      if (KettleInput >= KettleSetpoint)
      {
        KettleOutput = 0;
        StopPID();
        _heatOff = true;
      }
      else
        _kettlePID.Compute();
    else
      KettleOutput = 1023;

    KettleOutput = (KettleOutput * _activeStatus->RampPowerPercentage) / 100;
    _activeStatus->PWM = KettleOutput;
    Serial.print("PWM: ");
    Serial.println(_activeStatus->PWM);
  }
}

void KettleHeaterService::Compute()
{
  if (!_activeStatus->BrewStarted || _activeStatus->ActiveStep == none)
  {
    DisablePID();
    return;
  }
  if (_activeStatus->ActiveStep == boil)
  {
    _activeStatus->PWM = ((1023 * _activeStatus->BoilPowerPercentage) / 100);
    analogWrite(HEATER_BUS, _activeStatus->PWM);
    return;
  }

  checkHeatOff();
  generatePWM();

  analogWrite(HEATER_BUS, KettleOutput);
}