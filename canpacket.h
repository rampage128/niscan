#include <mcp_can.h>

#ifndef CANPACKET_H
#define CANPACKET_H

#define PK_OK               0
#define PK_INDEXOUTOFBOUNDS 1

#define PK_FLAG_ON  1
#define PK_FLAG_OFF 0

class CanPacket {
  public:
    static CanPacket fromMcp(MCP_CAN mcp);
    typedef enum STATUS { OK, INDEXOUTOFBOUNDS } PacketStatus;
    PacketStatus readByte(uint8_t index, unsigned char &buf);
    PacketStatus checkFlag(uint8_t index, unsigned char mask, unsigned char comparator, bool &result);
    PacketStatus writeFlag(uint8_t index, unsigned char mask, bool value);
    PacketStatus writeByte(uint8_t index, unsigned char value);
    long unsigned int getId();   
  private:
    long unsigned int rxId;
    unsigned char len = 0;
    unsigned char rxBuf[8];
    CanPacket();
    PacketStatus checkIndex(uint8_t index);
};

#endif
