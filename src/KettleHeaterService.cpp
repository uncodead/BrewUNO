#include <KettleHeaterService.h>

boolean _heatOff = true;
boolean _PIDPause = true;
double KettleSetpoint, KettleInput, KettleOutput;
PID _kettlePID = PID(&KettleInput, &KettleOutput, &KettleSetpoint, 1, 1, 1, DIRECT);
double _kp, _ki, _kd;

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
  _PIDPause = true;
  _kp = kp;
  _ki = ki;
  _kd = kd;
  _kettlePID.SetTunings(kp, ki, kd);
}

void KettleHeaterService::RestartPID()
{
  _kettlePID.SetMode(AUTOMATIC);

  _kettlePID.SetOutputLimits(0.0, 1.0);
  _kettlePID.SetOutputLimits(-1.0, 0.0);
  _kettlePID.SetOutputLimits(0, 1023);

  _kettlePID.SetMode(MANUAL);
}

void KettleHeaterService::DisablePID()
{
  RestartPID();
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

void KettleHeaterService::checkPauseMashPID()
{
  if (_activeStatus->ActiveStep == mash && _activeStatus->StartTime <= 0 && (_activeStatus->TargetTemperature - _activeStatus->Temperature) <= 2 && _PIDPause)
  {
    RestartPID();
    _heatOff = true;
    _PIDPause = false;
    _activeStatus->PWM = -1;
    KettleInput = _activeStatus->TargetTemperature + 5;
    analogWrite(HEATER_BUS, 0);
    delay(10000);
  }
}

void KettleHeaterService::setOriginalTuning()
{
  if (_activeStatus->StartTime <= 0)
  {
    _kettlePID.SetTunings(_kp, _ki, _kd);
    Serial.println("PID original");
  }
}

void KettleHeaterService::checkStepReached()
{
  if (_activeStatus->StepReached)
  {
    RestartPID();
    _heatOff = true;
    _PIDPause = true;
    _activeStatus->StepReached = false;
    _kettlePID.SetTunings(_kp + 10, _ki + 10, _kd + 10);
    Serial.println("PID agressive");
  }
}

void KettleHeaterService::Compute()
{
  if (!_activeStatus->BrewStarted || _activeStatus->ActiveStep == none)
  {
    DisablePID();
    return;
  }

  KettleInput = _activeStatus->Temperature;
  KettleSetpoint = _activeStatus->TargetTemperature;

  checkHeatOff();
  checkPauseMashPID();
  setOriginalTuning();
  checkStepReached();

  _kettlePID.Compute();

  _activeStatus->PWM = KettleOutput;
  Serial.print("PWM: ");
  Serial.println(_activeStatus->PWM);

  Serial.print("KPID: ");
  Serial.println(_kettlePID.GetKp() + ' ' + _kettlePID.GetKi() + ' ' + _kettlePID.GetKd());

  if (_activeStatus->ActiveStep == boil)
  {
    analogWrite(HEATER_BUS, (1023 * _activeStatus->BoilPowerPercentage) / 100);
    return;
  }

  analogWrite(HEATER_BUS, KettleOutput);
}