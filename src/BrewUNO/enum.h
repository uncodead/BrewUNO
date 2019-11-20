#ifndef enum_h
#define enum_h

enum StepType
{
  none,
  mash,
  boil,
  anticavitation
};


struct Temperatures
{
  float Main;
  float Sparge;
  float Boil;
  float AuxOne;
  float AuxTwo;
  float AuxThree;
};

#endif