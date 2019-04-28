#ifndef TemperatureService_h
#define TemperatureService_h

#include <OneWire.h>
#include <DallasTemperature.h>
#include <TimeLib.h>

class TemperatureService {
  public:
    TemperatureService(DallasTemperature dallasTemperature);
    ~TemperatureService();

    float GetTemperature();

  private:
    DallasTemperature _dallasTemperature;
};
#endif