#ifndef Pump_h
#define Pump_h

#include <Arduino.h>

class Pump
{
public:
  void TurnPump(bool on);
  void TurnPumpOn();
  void TurnPumpOff();
};

#endif
