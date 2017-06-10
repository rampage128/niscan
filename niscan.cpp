#include <arduino.h>

#include "niscan.h"
#include "niscan_dfs.h"

void NissanGearBoxCanConnector::readCan(CanPacket *packet) {
  if (packet->getId() == 0x421) {   
    GearBox *gbSystem = (GearBox*)_system;
    BinaryData* packetData = packet->getData();

    BinaryData::ByteResult gear = packetData->readByte(0);

    // gear is N (0) or R (-1)
    if (gear.data < 0x80) {
      gbSystem->setGear(((int8_t)gear.data / 8) - 3);
    }
    // gear is 1-6
    else {
      gbSystem->setGear(((int8_t)gear.data - 120) / 8);
    }

    gbSystem->setSynchroRev(packetData->readFlag(1, B01000000, B01000000).data);
  }
}

////

void NissanClimateControlCanConnector::readCan(CanPacket *packet) {
  ClimateControl *ccSystem = (ClimateControl*)_system;

  BinaryData* packetData = packet->getData();

  switch (packet->getId()) {
    case (NIS_AC1_IDX):
    ccSystem->setDesiredTemperature(packetData->readByte(NIS_AC1_TEMP_IDX).data);
    break;
    case (NIS_AC2_IDX):
    
    ccSystem->setFanLevel((packetData->readByte(NIS_AC2_FANS_IDX).data - 4) / 8);
    switch (packetData->readByte(NIS_AC2_MODE_IDX).data) {
      case (NIS_AC2_MODE_OFF):
      ccSystem->setAirductWindshield(false);
      ccSystem->setAirductFace(false);
      ccSystem->setAirductFeet(false);
      break;
      case (NIS_AC2_MODE_FEET):
      ccSystem->setAirductWindshield(false);
      ccSystem->setAirductFace(false);
      ccSystem->setAirductFeet(true);
      break;
      case (NIS_AC2_MODE_WINDFEET):
      ccSystem->setAirductWindshield(true);
      ccSystem->setAirductFace(false);
      ccSystem->setAirductFeet(true);
      break;
      case (NIS_AC2_MODE_FACE):
      ccSystem->setAirductWindshield(false);
      ccSystem->setAirductFace(true);
      ccSystem->setAirductFeet(false);
      break;
      case (NIS_AC2_MODE_FACEFEET):
      ccSystem->setAirductWindshield(false);
      ccSystem->setAirductFace(true);
      ccSystem->setAirductFeet(true);
      break;
    }
    ccSystem->setWindshieldHeating(packetData->readFlag(NIS_AC2_WIND_IDX, NIS_AC2_WIND_MSK, NIS_AC2_WIND_ON).data);
    ccSystem->setRecirculation(packetData->readFlag(NIS_AC2_RECI_IDX, NIS_AC2_RECI_MSK, NIS_AC2_RECI_ON).data);
    ccSystem->setAuto(packetData->readFlag(NIS_AC2_AUTO_IDX, NIS_AC2_AUTO_MSK, NIS_AC2_AUTO_ON).data);
    
    break;
    case (NIS_AC3_IDX):
    ccSystem->setAc(packetData->readFlag(NIS_AC3_AC_IDX, NIS_AC3_AC_MSK, NIS_AC3_AC_ON).data);
    break;
    case (NIS_AC4_IDX):
    ccSystem->setRearWindowHeating(packetData->readFlag(NIS_AC4_RH_IDX, NIS_AC4_RH_MSK, NIS_AC4_RH_ON).data);
    break;
  }
}

void NissanClimateControlCanConnector::writeCan(MCP_CAN* can) {

  _can1.writeByte(7, _roll);
  _can2.writeByte(7, _roll);
  _can3.writeByte(7, _roll);

  can->sendMsgBuf(0x540, 0, 8, _can1.getData());
  can->sendMsgBuf(0x541, 0, 8, _can2.getData());
  can->sendMsgBuf(0x542, 0, 8, _can3.getData());

  // reset rear heating flag after sending ... this is a special case.
  _can2.writeFlag(1, B10000000, B00000000);

  if (_roll < 3) {
    _roll++;
  }
  else {
    _roll = 0;
  }
}

void NissanClimateControlCanConnector::pressModeButton() {
  _can2.toggleFlag(0, B00010000);
}

void NissanClimateControlCanConnector::pressWindshieldButton() {
  _can2.toggleFlag(0, B00000001);
}

void NissanClimateControlCanConnector::pressRearHeaterButton() {
  // always write to 1 ... rear heating is different from all other flags
  _can2.writeFlag(1, B10000000, B10000000);
}

void NissanClimateControlCanConnector::pressRecirculationButton() {
  _can2.toggleFlag(1, B10000000);
}

void NissanClimateControlCanConnector::setTemperature(uint8_t temperature) {
  _can2.toggleFlag(2, B10000000);
  _can3.writeByte(1, temperature);
}

void NissanClimateControlCanConnector::pressAcButton() {
  _can2.toggleFlag(2, B00000100);
}

void NissanClimateControlCanConnector::pressOffButton() {
  _can2.toggleFlag(2, B00000001);
}

void NissanClimateControlCanConnector::pressAutoButton() {
  _can2.toggleFlag(3, B10000000);
}

void NissanClimateControlCanConnector::setFanSpeed(uint8_t fanSpeed) {
  _can2.toggleFlag(3, B00010000);
  _can3.writeByte(0, fanSpeed * 8 + 4);
}

