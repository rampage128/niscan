#ifndef CANPACKET_H
#define CANPACKET_H

#include <mcp_can.h>
#include "binarydata.h"

class CanPacket {
  public:
    static CanPacket fromMcp(MCP_CAN mcp);
    ~CanPacket() {
      free(_data);
    }
    BinaryData* getData();
    long unsigned int getId();   
  private:
    CanPacket(long unsigned int id, uint8_t len, byte data[8]);
    long unsigned int _id;
    BinaryData* _data;
};

#endif
