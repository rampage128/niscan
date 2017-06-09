#ifndef BINARYDATA_H
#define BINARYDATA_H

#include <stdint.h>

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
    AccessStatus writeFlag(uint8_t index, unsigned char mask, bool value);
    AccessStatus writeByte(uint8_t index, unsigned char value);
    BoolResult readFlag(uint8_t index, unsigned char mask, unsigned char comparator);
    ByteResult readByte(uint8_t index);
    char* getData();
    uint8_t getSize();
  private:
    uint8_t _len = 0;
    char* _payload;
    AccessStatus checkIndex(uint8_t index);
};

#endif
