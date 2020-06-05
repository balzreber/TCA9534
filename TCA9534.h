#ifndef TCA9534_h
#define TCA9534_h

#include "Arduino.h"

class TCA9534
{

  public:
    TCA9534(int _ADR);
    int Begin(void);
    int PinMode(int Pin, boolean PinType);
    int DigitalWrite(int Pin, boolean State);
    int DigitalRead(int Pin);

  private:
    int ADR;
  	uint8_t PinModeConf;
    uint8_t Port;
    uint8_t PinPolarityConfig;

    int SetPort(int Config);
    int SetDirection(int Config);
};

#endif
