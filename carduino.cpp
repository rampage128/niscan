#include <arduino.h>
#include "carduino.h"

void CarSystem::serialize() {
  uint8_t packetSize = _state->getSize();
  char* data = _state->getData();

  Serial.write("{s");
  Serial.write(_id);
  Serial.write(packetSize);
  for (int i = 0; i < packetSize; i++) {
    Serial.write(data[i]);
  }
  Serial.write("}");
}

//////////////////////////////////////////

void ClimateControl::setAc(bool state) {
  _state->writeFlag(0, 0x80, state);
}

void ClimateControl::setAuto(bool state) {
  _state->writeFlag(0, 0x40, state);
}

void ClimateControl::setAirductWindshield(bool state) {
  _state->writeFlag(0, 0x20, state);
}

void ClimateControl::setAirductFace(bool state) {
  _state->writeFlag(0, 0x10, state);
}

void ClimateControl::setAirductFeet(bool state) {
  _state->writeFlag(0, 0x08, state);
}

void ClimateControl::setWindshieldHeating(bool state) {
  _state->writeFlag(0, 0x04, state);
}

void ClimateControl::setRearWindowHeating(bool state) {
  _state->writeFlag(0, 0x02, state);
}

void ClimateControl::setRecirculation(bool state) {
  _state->writeFlag(0, 0x01, state);
}

void ClimateControl::setFanLevel(uint8_t fans) {
  _state->writeByte(1, fans);
}

void ClimateControl::setDesiredTemperature(uint8_t temperature) {
  _state->writeByte(2, temperature);
}

//////////////////////////////////////////

void GearBox::setGear(int8_t gearNum) {
  _state->writeByte(0, gearNum);
}

void GearBox::setSynchroRev(bool state) {
  _state->writeFlag(0, 0x80, state);
}
