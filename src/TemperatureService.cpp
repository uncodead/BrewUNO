#include <TemperatureService.h>

TemperatureService::TemperatureService(DallasTemperature dallasTemperature) : _dallasTemperature(dallasTemperature)
{
}

TemperatureService::~TemperatureService() {}

float TemperatureService::GetTemperature()
{
  float tempC = 0;
  _dallasTemperature.requestTemperatures();
  while (tempC <= 0) {
    tempC = _dallasTemperature.getTempCByIndex(0);
    yield();
  }
  return tempC;
}