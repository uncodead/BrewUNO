#ifndef Pump_h
#define Pump_h

#include <Arduino.h>
#include <TimeLib.h>
#include <BrewUNO/BrewSettingsService.h>
#include <BrewUNO/ActiveStatus.h>

class Pump
{
public:
  Pump(ActiveStatus *activeStatus, BrewSettingsService *brewSettingsService);

  BrewSettingsService *_brewSettingsService;
  ActiveStatus *_activeStatus;
  
  void TurnPumpOn();
  void TurnPumpOff();
  void CheckRest();
  void AntiCavitation();

private:
  void TurnPump(bool on);
};

#endif
