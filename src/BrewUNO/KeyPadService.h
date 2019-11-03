#ifndef KeyPadService_h
#define KeyPadService_h

#include <BrewUNO/BrewSettingsService.h>
#include <BrewUNO/ActiveStatus.h>
#include <BrewUNO/KeyButton.h>
#include <BrewUNO/Buzzer.h>
#include <BrewUNO/Pump.h>
#include <BrewUNO/BrewService.h>
#include <BrewUNO/BrewSettingsService.h>

#include <pcf8574_esp.h>

class KeyPadService
{
public:
  KeyPadService(ActiveStatus *activeStatus, PCF857x *pcf, BrewService *brewService, BrewSettingsService *brewSettingsService, Pump *pump,
                KeyButton *button1, KeyButton *button2, KeyButton *button3, KeyButton *button4);

  ActiveStatus *_activeStatus;
  BrewSettingsService *_brewSettingsService;
  PCF857x *_pcf;

  KeyButton *_button1;
  KeyButton *_button2;
  KeyButton *_button3;
  KeyButton *_button4;

  BrewService *_brewService;
  Pump *_pump;

  void loop(bool PCFInterruptFlag);
};

#endif