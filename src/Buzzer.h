#ifndef Buzzer_h
#define Buzzer_h

#include <Arduino.h>

class Buzzer
{
public:
  static void Ring();
  static void Ring(int);
  static void Ring(int count, int duration);
};

#endif
