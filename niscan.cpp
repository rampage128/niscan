#include <arduino.h>

#include "niscan.h"
#include "niscan_dfs.h"

void NissanGearBoxPopulator::populate(CanPacket *packet) {
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

void NissanClimateControlPopulator::populate(CanPacket *packet) {
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

