#include <arduino.h>
#include "carduino.h"

CarData::CarData(uint8_t len) {
  _len = len;
  _payload = (char*)calloc(sizeof(char), _len+1);
}

CarData::AccessStatus CarData::readFlag(uint8_t index, unsigned char mask, bool &result) {
  if (checkIndex(index) == AccessStatus::INDEXOUTOFBOUNDS) {
    return AccessStatus::INDEXOUTOFBOUNDS;
  }

  result = (_payload[index] & mask) == mask;
  return AccessStatus::OK;
}

CarData::AccessStatus CarData::readByte(uint8_t index, unsigned char &buf) {
  if (checkIndex(index) == AccessStatus::INDEXOUTOFBOUNDS) {
    return AccessStatus::INDEXOUTOFBOUNDS;
  }

  buf = _payload[index];
  return AccessStatus::OK;
}

CarData::AccessStatus CarData::setFlag(uint8_t index, unsigned char mask, bool value) {
  if (checkIndex(index) == AccessStatus::INDEXOUTOFBOUNDS) {
    return AccessStatus::INDEXOUTOFBOUNDS;
  }

  if (value) {
    _payload[index] |= mask;
  } 
  else {
    _payload[index] &= ~mask;
  }
  return AccessStatus::OK;
}

CarData::AccessStatus CarData::setByte(uint8_t index, unsigned char value) {
  if (checkIndex(index) == AccessStatus::INDEXOUTOFBOUNDS) {
    return AccessStatus::INDEXOUTOFBOUNDS;
  }

  _payload[index] = value;
  return STATUS::OK;
}

CarData::AccessStatus CarData::checkIndex(uint8_t index) {
    if (_len <= index || index < 0) {
    return AccessStatus::INDEXOUTOFBOUNDS;
  }
  return AccessStatus::OK;
}

uint8_t CarData::getSize() {
  return _len;
}

char* CarData::serialize() {
  return _payload;
}

//////////////////////////////////////////

CarSystem::CarSystem(uint8_t id, uint8_t len) {
  _id = id;
  _data = new CarData(len);
}

void CarSystem::serialize() {
  uint8_t packetSize = _data->getSize();
  char* data = _data->serialize();

  Serial.write(">s");
  Serial.write(_id);
  Serial.write(packetSize);
  for (int i = 0; i < packetSize; i++) {
    Serial.write(data[i]);
  }
  Serial.write("|");
}

//////////////////////////////////////////

void ClimateControl::setAc(bool state) {
  _data->setFlag(0, 0x80, state);
}

void ClimateControl::setAuto(bool state) {
  _data->setFlag(0, 0x40, state);
}

void ClimateControl::setAirductWindshield(bool state) {
  _data->setFlag(0, 0x20, state);
}

void ClimateControl::setAirductFace(bool state) {
  _data->setFlag(0, 0x10, state);
}

void ClimateControl::setAirductFeet(bool state) {
  _data->setFlag(0, 0x08, state);
}

void ClimateControl::setWindshieldHeating(bool state) {
  _data->setFlag(0, 0x04, state);
}

void ClimateControl::setRearWindowHeating(bool state) {
  _data->setFlag(0, 0x02, state);
}

void ClimateControl::setRecirculation(bool state) {
  _data->setFlag(0, 0x01, state);
}

void ClimateControl::setFanLevel(uint8_t fans) {
  _data->setByte(1, fans);
}

void ClimateControl::setDesiredTemperature(uint8_t temperature) {
  _data->setByte(2, temperature);
}

//////////////////////////////////////////

void GearBox::setGear(int8_t gearNum) {
  _data->setByte(0, gearNum);
}

void GearBox::setSynchroRev(bool state) {
  _data->setFlag(0, 0x80, state);
}
