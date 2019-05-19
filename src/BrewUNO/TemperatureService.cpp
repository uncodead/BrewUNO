#include <BrewUNO/TemperatureService.h>

TemperatureService::TemperatureService(DallasTemperature dallasTemperature) : _dallasTemperature(dallasTemperature)
{
}

TemperatureService::~TemperatureService() {}

time_t lastRead = now();

float TemperatureService::GetTemperature()
{
  if (now() - lastRead < 1)
    return 0;

  lastRead = now();
  float tempC = 0;
  _dallasTemperature.requestTemperatures();
  tempC = _dallasTemperature.getTempCByIndex(0);
  return tempC;
}