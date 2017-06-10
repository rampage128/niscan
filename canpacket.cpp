#include "canpacket.h"

CanPacket::CanPacket(long unsigned int id, unsigned char len, unsigned char rxBuf[8]) {
  _id = id;
  _data = new BinaryData(len);
  for (uint8_t i = 0; i < len; i++) {
    _data->writeByte(i, rxBuf[i]);
  }
}

CanPacket CanPacket::fromMcp(MCP_CAN mcp) {
  long unsigned int id = 0;
  unsigned char len = 0;
  unsigned char rxBuf[8];
  
  mcp.readMsgBuf(&id, &len, rxBuf); 

  CanPacket packet(id, len, rxBuf);
  
  return packet;
}

BinaryData* CanPacket::getData() {
  return _data;
}

long unsigned int CanPacket::getId() {
  return _id;
}

