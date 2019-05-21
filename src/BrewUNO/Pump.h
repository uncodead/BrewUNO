#ifndef Pump_h
#define Pump_h

#include <Arduino.h>
#include <TimeLib.h>
#include <BrewUNO/BrewSettingsService.h>

class Pump
{
public:
  Pump(BrewSettingsService *brewSettingsService);

  BrewSettingsService *_brewSettingsService;
  
  void TurnPumpOn();
  void TurnPumpOff();
  void CheckRest();
  void AntiCavitation();

private:
  void TurnPump(bool on);
};

#endif
