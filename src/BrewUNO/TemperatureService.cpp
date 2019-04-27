#include <BrewUNO/TemperatureService.h>

TemperatureService::TemperatureService(DallasTemperature dallasTemperature) : _dallasTemperature(dallasTemperature)
{
}

TemperatureService::~TemperatureService() {}

float TemperatureService::GetTemperature()
{
  float tempC = 0;
  _dallasTemperature.requestTemperatures();
  tempC = _dallasTemperature.getTempCByIndex(0);
  return tempC;
}