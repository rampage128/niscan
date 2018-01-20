#include <arduino.h>
#include "binarydata.h"

BinaryData::BinaryData(uint8_t len) {
  _len = len;
  _payload = (char*)calloc(sizeof(char), _len+1);
}

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

BinaryData::LongResult BinaryData::readLong(uint8_t index) {
  struct LongResult result;

  if (checkIndex(index + 4) == AccessStatus::INDEXOUTOFBOUNDS) {
    result.state = AccessStatus::INDEXOUTOFBOUNDS;
    return result;
  }

  result.data = (unsigned long)ntohl(*((long *)&_payload[index]));
  result.state = AccessStatus::OK;
  return result;
}

BinaryData::AccessStatus BinaryData::toggleFlag(uint8_t index, unsigned char mask) {
  if (checkIndex(index) == AccessStatus::INDEXOUTOFBOUNDS) {
    return AccessStatus::INDEXOUTOFBOUNDS;
  }

  _payload[index] ^= mask;

  return AccessStatus::OK;
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
  return AccessStatus::OK;
}

BinaryData::AccessStatus BinaryData::writeData(uint8_t index, BinaryData* data, uint8_t from, uint8_t to) {
  int inputLen = to - from;

  if (checkIndex(index) == AccessStatus::INDEXOUTOFBOUNDS) {
    return AccessStatus::INDEXOUTOFBOUNDS;
  }
  if (checkIndex(index + inputLen - 1) == AccessStatus::INDEXOUTOFBOUNDS) {
    return AccessStatus::INDEXOUTOFBOUNDS;
  }

  char* inputData = data->_payload;

  for (int i = 0; i < inputLen; i++) {
    _payload[index + i] = inputData[from + i];
  }

  return AccessStatus::OK;
}

BinaryData::AccessStatus BinaryData::checkIndex(uint8_t index) {
    if (_len <= index) {
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
