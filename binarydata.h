#ifndef BINARYDATA_H
#define BINARYDATA_H

#include <stdint.h>
#include "network.h"

class BinaryData {
  public:
    BinaryData(uint8_t len) {
      _len = len;
      _payload = (char*)calloc(sizeof(char), _len+1);
    };
    virtual ~BinaryData();
    typedef enum STATUS {
      OK,
      INDEXOUTOFBOUNDS
    } AccessStatus;
    struct ByteResult {
      AccessStatus state;
      unsigned char data;
    };
    struct BoolResult {
      AccessStatus state;
      bool data;
    };
    struct LongResult {
      AccessStatus state;
      unsigned long int data;
    };
    AccessStatus toggleFlag(uint8_t index, unsigned char mask);
    AccessStatus writeFlag(uint8_t index, unsigned char mask, bool value);
    AccessStatus writeByte(uint8_t index, unsigned char value);
    AccessStatus writeData(uint8_t index, BinaryData* data);
    BoolResult readFlag(uint8_t index, unsigned char mask, unsigned char comparator);
    ByteResult readByte(uint8_t index);
    LongResult readLong(uint8_t index);
    char* getData();
    uint8_t getSize();
  private:
    uint8_t _len = 0;
    char* _payload;
    AccessStatus checkIndex(uint8_t index);
};

class BinaryBuffer {
  public:
    BinaryBuffer(uint8_t len) {
      _data = new BinaryData(len);
    }
    ~BinaryBuffer() {
      delete _data;
    }
    int available() {
      return _data->getSize() - _position;
    }
    BinaryData::ByteResult peek() {
      return _data->readByte(_position);
    }
    BinaryData::ByteResult readByte() {
      BinaryData::ByteResult result = _data->readByte(_position);
      if (result.state == BinaryData::OK) {
        _position++;
      }
      return result;
    }
    BinaryData::LongResult readLong() {
      BinaryData::LongResult result = _data->readLong(_position);
      if (result.state == BinaryData::OK) {
        _position++;
      }
      return result;
    }
    BinaryData::AccessStatus write(unsigned char value) {
      BinaryData::AccessStatus result = _data->writeByte(_position, value);
      if (result == BinaryData::OK) {
        _position++;
      }
      return result;
    }
    boolean goTo(int index) {
      if (index < _data->getSize() - 1) {
        _position = index;
        return true;
      }
      return false;
    }
    uint8_t getPosition() {
      return _position;
    }
    boolean next() {
      if (_position < _data->getSize() - 1) {
        _position++;
        return true;
      }
      return false;
    }
    uint8_t getSize() {
      return _data->getSize();
    }
  private:
    BinaryData* _data;
    uint8_t _position = 0;
};

#endif
