#include <stdint.h>
#include "canpacket.h"

CanPacket::CanPacket(void) {
  // INTENTIONALLY LEFT BLANK
}

CanPacket CanPacket::fromMcp(MCP_CAN mcp) {
  CanPacket packet;
  mcp.readMsgBuf(&packet.rxId, &packet.len, packet.rxBuf); 
  return packet;
}

CanPacket::PacketStatus CanPacket::readByte(uint8_t index, unsigned char &buf) {
  if (checkIndex(index) == STATUS::INDEXOUTOFBOUNDS) {
    return STATUS::INDEXOUTOFBOUNDS;
  }
  
  buf = rxBuf[index];
  return STATUS::OK;
}

CanPacket::PacketStatus CanPacket::checkFlag(uint8_t index, unsigned char mask, unsigned char comparator, bool &result) {
  if (checkIndex(index) == STATUS::INDEXOUTOFBOUNDS) {
    return STATUS::INDEXOUTOFBOUNDS;
  }
  
  unsigned char tBuf = rxBuf[index] & mask; 
  result = tBuf == comparator;

  return STATUS::OK;
}

CanPacket::PacketStatus CanPacket::writeFlag(uint8_t index, unsigned char mask, bool value) {
  if (checkIndex(index) == STATUS::INDEXOUTOFBOUNDS) {
    return STATUS::INDEXOUTOFBOUNDS;
  }

  if (value) {
    rxBuf[index] |= mask;
  } 
  else {
    rxBuf[index] &= ~mask;
  }
  return STATUS::OK;
}

CanPacket::PacketStatus CanPacket::writeByte(uint8_t index, unsigned char value) {
  if (checkIndex(index) == PacketStatus::INDEXOUTOFBOUNDS) {
    return PacketStatus::INDEXOUTOFBOUNDS;
  }

  rxBuf[index] = value;
  return PacketStatus::OK;
}

CanPacket::PacketStatus CanPacket::checkIndex(uint8_t index) {
  if (len <= index || index < 0) {
    return PacketStatus::INDEXOUTOFBOUNDS;
  }
  return PacketStatus::OK;
}

long unsigned int CanPacket::getId() {
  return rxId;
}

