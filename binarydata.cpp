#include <arduino.h>
#include "binarydata.h"



BinaryData::~BinaryData() {
  free(_payload);
}

BinaryData::BoolResult BinaryData::readFlag(uint8_t index, unsigned char mask, unsigned char comparator) {
  struct BoolResult result;
  
  if (checkIndex(index) == AccessStatus::INDEXOUTOFBOUNDS) {
    result.state = AccessStatus::INDEXOUTOFBOUNDS;
    return result;
  }

  result.data = (_payload[index] & mask) == comparator;
  result.state = AccessStatus::OK;
  return result;
}

BinaryData::ByteResult BinaryData::readByte(uint8_t index) {
  struct ByteResult result;
  
  if (checkIndex(index) == AccessStatus::INDEXOUTOFBOUNDS) {
    result.state = AccessStatus::INDEXOUTOFBOUNDS;
    return result;
  }

  result.data = _payload[index];
  result.state = AccessStatus::OK;
  return result;
}

BinaryData::AccessStatus BinaryData::writeFlag(uint8_t index, unsigned char mask, bool value) {
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

BinaryData::AccessStatus BinaryData::writeByte(uint8_t index, unsigned char value) {
  if (checkIndex(index) == AccessStatus::INDEXOUTOFBOUNDS) {
    return AccessStatus::INDEXOUTOFBOUNDS;
  }

  _payload[index] = value;
  return STATUS::OK;
}

BinaryData::AccessStatus BinaryData::checkIndex(uint8_t index) {
    if (_len <= index || index < 0) {
    return AccessStatus::INDEXOUTOFBOUNDS;
  }
  return AccessStatus::OK;
}

uint8_t BinaryData::getSize() {
  return _len;
}

char* BinaryData::getData() {
  return _payload;
}
