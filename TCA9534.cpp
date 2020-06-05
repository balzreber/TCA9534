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
  if(Pin > 8 || Pin < 0) {
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

  // Returns -1 if pin type was not or wrongly specified
  else return -1;

}

// Write state to specified pin
int TCA9534::DigitalWrite(int Pin, boolean State)
{

  // Check if specified pin is in range of avalable pins
  if(Pin > 8 || Pin < 0) {
    return -1;
  }

  if(State == HIGH)
  {
    Port = Port | (0x01 << Pin);
    SetPort(Port);
    return 1;
  }
  else if(State == LOW)
  {
    Port = Port & ~(0x01 << Pin);
    SetPort(Port);
    return 0;
  }
  else
    return -1; //Fail if state is ill-defined
}

int TCA9534::DigitalRead(int Pin)
{
  if(Pin > 8 || Pin < 0)
  {
    return -1; //Fail if pin out of range
  }

  Wire.beginTransmission(ADR);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(ADR, Pin);
  Wire.requestFrom(ADR, 1);
  int inRegister =  Wire.read();
  return (( -inRegister >> Pin) & 0x01 );
}

int TCA9534::SetPort(int Config) {
  Wire.beginTransmission(ADR); // transmit to device with address ADR
  Wire.write(0x01);   //Send to output set register
  Wire.write(Config);
  return Wire.endTransmission();
}


int TCA9534::SetDirection(int Config) {
  Wire.beginTransmission(ADR); // transmit to device with address ADR
  Wire.write(0x03);        //Send to port configuration register
  Wire.write(Config);
  return Wire.endTransmission();    // stop transmitting
}
