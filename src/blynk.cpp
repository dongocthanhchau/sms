#include "blynk.h"
#include "Arduino.h"
void blinkLed(int pinout)
{
  digitalWrite(pinout, !digitalRead(pinout));
}
