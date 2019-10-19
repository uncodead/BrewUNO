#include <BrewUNO/KeyPadService.h>

KeyPadService::KeyPadService(ActiveStatus *activeStatus, PCF857x *pcf, BrewService *brewService, Pump *pump,
                             KeyButton *button1, KeyButton *button2, KeyButton *button3, KeyButton *button4)
    : _pcf(pcf),
      _activeStatus(activeStatus),
      _brewService(brewService),
      _pump(pump),
      _button1(button1), _button2(button2), _button3(button3), _button4(button4)
{
}

void KeyPadService::loop()
{
  _button1->Update();
  _button2->Update();
  _button3->Update();
  _button4->Update();

  if (_button1->pressed_long && !_activeStatus->BrewStarted)
    _brewService->startBrew();

  if (_button1->pressed)
  {
    if (_activeStatus->BrewStarted)
      _brewService->pauseBrew();
    else if (_activeStatus->ActiveStep > 0 && _activeStatus->ActiveStep != 3)
      _brewService->resumeBrew();
  }

  if (_button2->pressed_long && _activeStatus->BrewStarted)
    _brewService->stopBrew();

  if (_button3->pressed && _activeStatus->BrewStarted)
    _brewService->nextStep();

  if (_button3->pressed_long)
    _brewService->startBoil();

  if (_button4->pressed)
    _pump->TurnPump(!_activeStatus->PumpOn);
}