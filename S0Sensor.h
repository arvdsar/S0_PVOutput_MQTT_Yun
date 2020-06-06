#ifndef S0_h
#define S0_h

#include "BaseSensor.h"

class S0Sensor : public BaseSensor
{
  public:
    S0Sensor(byte pin, int p, long sid, byte v, int f);
    void Begin(byte i);
    void CheckSensor();
  
  private:
    bool sensorIsOn;
    byte pin;
};

#endif
