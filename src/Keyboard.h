#ifndef Keyboard_h
#define Keyboard_h

#include <BrewSettingsService.h>
#include <ActiveStatus.h>
#include <KeyButton.h>
#include <Buzzer.h>
#include <Pump.h>
#include <BrewUnoService.h>
#include <BrewSettingsService.h>

#include <pcf8574_esp.h>

class Keyboard
{
public:
  Keyboard(ActiveStatus *activeStatus, PCF857x *pcf, BrewUnoService *brewService, BrewSettingsService *brewSettingsService, Pump *pump,
           KeyButton *button1, KeyButton *button2, KeyButton *button3, KeyButton *button4);

  ActiveStatus *_activeStatus;
  BrewSettingsService *_brewSettingsService;
  PCF857x *_pcf;

  KeyButton *_button1;
  KeyButton *_button2;
  KeyButton *_button3;
  KeyButton *_button4;

  BrewUnoService *_brewService;
  Pump *_pump;

  void update(bool PCFInterruptFlag);
};

#endif