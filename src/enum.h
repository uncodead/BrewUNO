#ifndef enum_h
#define enum_h

enum StepType
{
  none,
  mash,
  boil,
  cooling,
  anticavitation
};


struct Temperatures
{
  float Main;
  float Sparge;
  float Boil;
  float Cooling;
  float AuxOne;
  float AuxTwo;
  float AuxThree;
};

#endif