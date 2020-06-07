#include <Arduino.h>
#include <TCA9534.h>
#include <Wire.h>

TCA9534::TCA9534(int _ADR) {
  ADR = _ADR;
  Wire.begin();
}

// Begin comms with the device and test for presence
int TCA9534::Begin(void) {
  Wire.beginTransmission(ADR);
  if(Wire.endTransmission() != 0) return -1;
  else return 1;
}

// Set pin mode to input or output
int TCA9534::PinMode(int Pin, boolean PinType) {

  // Check if specified pin is in range of avalable pins
  if(Pin > 7 || Pin < 0) {
    return -1;
  }

  // Set pin to input
  if(PinType == INPUT) {
    PinModeConf = PinModeConf | (0x01 << Pin);
    SetDirection(PinModeConf);
    return 1;
  }

  // Set pin to output
  else if(PinType == OUTPUT)
  {
    PinModeConf = PinModeConf & ~(0x01 << Pin);
    SetDirection(PinModeConf);
    return 0;
  }

  // Returns -1 (fail) if pin type was not or wrongly specified
  else return -1;

}

// Write state to specified pin
int TCA9534::DigitalWrite(int Pin, boolean State)
{

  // Check if specified pin is in range of avalable pins
  if(Pin > 7 || Pin < 0) {
    return -1;
  }

  // If state is HIGH write according shifted port via SetPort()
  if(State == HIGH) {
    Port = Port | (0x01 << Pin);
    SetPort(Port);
    return 1;
  }

  // If state is LOW write according shifted port via SetPort()
  else if(State == LOW) {
    Port = Port & ~(0x01 << Pin);
    SetPort(Port);
    return 0;
  }

  // Returns -1 (fail) if pin type was not or wrongly specified
  else return -1;

}

// Read state from specified pin
int TCA9534::DigitalRead(int Pin)
{

  // Check if specified pin is in range of avalable pins
  if(Pin > 7 || Pin < 0) {
    return -1;
  }

  // Initiate transmission, set input port and the read from register
  Wire.beginTransmission(ADR);
  Wire.write(0x00); // 0x00 = Input Port
  Wire.endTransmission();
  Wire.requestFrom(ADR, 1);
  int inRegister =  Wire.read();
  return (( -inRegister >> Pin) & 0x01 );
}

// Set port via output register (0x01)
int TCA9534::SetPort(int Config) {
  Wire.beginTransmission(ADR);
  Wire.write(0x01); // 0x01 = Output Port
  Wire.write(Config);
  return Wire.endTransmission();
}

// Set direction via configuration register (0x03)
int TCA9534::SetDirection(int Config) {
  Wire.beginTransmission(ADR);
  Wire.write(0x03); // 0x03 = Configuration
  Wire.write(Config);
  return Wire.endTransmission();
}
