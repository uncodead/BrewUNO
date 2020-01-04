#include <Keyboard.h>

Keyboard::Keyboard(ActiveStatus *activeStatus, PCF857x *pcf, BrewService *brewService, BrewSettingsService *brewSettingsService, Pump *pump,
                             KeyButton *button1, KeyButton *button2, KeyButton *button3, KeyButton *button4)
    : _pcf(pcf),
      _activeStatus(activeStatus),
      _brewService(brewService),
      _brewSettingsService(brewSettingsService),
      _pump(pump),
      _button1(button1), _button2(button2), _button3(button3), _button4(button4)
{
}

void Keyboard::update(bool PCFInterruptFlag)
{
  if (PCFInterruptFlag)
  {
    _button1->Update();
    _button2->Update();
    _button3->Update();
    _button4->Update();

    if (_button1->pressed_long)
    {
      if (!_activeStatus->BrewStarted)
        _brewService->startBrew();
      else
        _brewService->stopBrew();
      Buzzer().Ring(1, 100);
    }

    if (_button1->pressed)
    {
      if (_activeStatus->BrewStarted)
        _brewService->pauseBrew();
      else if (_activeStatus->ActiveStep > 0 && _activeStatus->ActiveStep != 3)
        _brewService->resumeBrew();
      Buzzer().Ring(1, 100);
    }

    if (_button2->pressed_long)
    {
      _brewService->startBoil();
      Buzzer().Ring(1, 100);
    }

    if (_button2->pressed)
    {
      if (_activeStatus->BrewStarted && _activeStatus->ActiveStep == mash)
      {
        _brewSettingsService->MashHeaterPercentage -= 10;
        if (_brewSettingsService->MashHeaterPercentage < 0)
          _brewSettingsService->MashHeaterPercentage = 0;
      }
      else if (_activeStatus->BrewStarted && _activeStatus->ActiveStep == boil)
      {
        _brewSettingsService->BoilPowerPercentage -= 10;
        if (_brewSettingsService->BoilPowerPercentage < 0)
          _brewSettingsService->BoilPowerPercentage = 0;
      }
      Buzzer().Ring(1, 100);
    }

    if (_button3->pressed_long && _activeStatus->BrewStarted)
    {
      _brewService->nextStep();
      Buzzer().Ring(1, 100);
    }

    if (_button3->pressed)
    {
      if (_activeStatus->BrewStarted && _activeStatus->ActiveStep == mash)
      {
        _brewSettingsService->MashHeaterPercentage += 10;
        if (_brewSettingsService->MashHeaterPercentage > 100)
          _brewSettingsService->MashHeaterPercentage = 100;
      }
      else if (_activeStatus->BrewStarted && _activeStatus->ActiveStep == boil)
      {
        _brewSettingsService->BoilPowerPercentage += 10;
        if (_brewSettingsService->BoilPowerPercentage > 100)
          _brewSettingsService->BoilPowerPercentage = 100;
      }
      Buzzer().Ring(1, 100);
    }

    if (_button4->pressed)
    {
      _pump->TurnPump(!_activeStatus->PumpOn);
      Buzzer().Ring(1, 100);
    }
  }
}