#include <arduino.h>

#include "niscan.h"
#include "niscan_dfs.h"

void NissanGearBoxPopulator::populate(CanPacket *packet) {
  if (packet->getId() == 0x421) {
    GearBox *gbSystem = (GearBox*)_system;

    unsigned char gear = 0;
    bool isSynchroRev = false;

    packet->readByte(0, gear);
    packet->checkFlag(1, B01000000, B01000000, isSynchroRev);

    // gear is N (0) or R (-1)
    if (gear < 0x80) {
      gbSystem->setGear(((int8_t)gear / 8) - 3);
    }
    // gear is 1-6
    else {
      gbSystem->setGear(((int8_t)gear - 120) / 8);
    }

    gbSystem->setSynchroRev(isSynchroRev);
  }
}

void NissanClimateControlPopulator::populate(CanPacket *packet) {
  ClimateControl *ccSystem = (ClimateControl*)_system;
  
  unsigned char temp = 0;

  unsigned char mode;
  bool isAutomatic = false;
  bool isWindshieldOn = false;
  bool isReciOn = false;
  unsigned char fans;

  bool isAcOn = false;
  
  bool isRearHeaterOn = false;
  
  switch (packet->getId()) {
    case (NIS_AC1_IDX):
    packet->readByte(NIS_AC1_TEMP_IDX, temp);
    ccSystem->setDesiredTemperature(temp);
    break;
    case (NIS_AC2_IDX):
    packet->readByte(NIS_AC2_MODE_IDX, mode);
    packet->checkFlag(NIS_AC2_WIND_IDX, NIS_AC2_WIND_MSK, NIS_AC2_WIND_ON, isWindshieldOn);
    packet->checkFlag(NIS_AC2_RECI_IDX, NIS_AC2_RECI_MSK, NIS_AC2_RECI_ON, isReciOn);
    packet->checkFlag(NIS_AC2_AUTO_IDX, NIS_AC2_AUTO_MSK, NIS_AC2_AUTO_ON, isAutomatic);
    packet->readByte(NIS_AC2_FANS_IDX, fans);
    switch (mode) {
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
    ccSystem->setWindshieldHeating(isWindshieldOn);
    ccSystem->setRecirculation(isReciOn);
    ccSystem->setAuto(isAutomatic);
    ccSystem->setFanLevel((fans - 4) / 8);
    break;
    case (NIS_AC3_IDX):
    packet->checkFlag(NIS_AC3_AC_IDX, NIS_AC3_AC_MSK, NIS_AC3_AC_ON, isAcOn);
    ccSystem->setAc(isAcOn);
    break;
    case (NIS_AC4_IDX):
    packet->checkFlag(NIS_AC4_RH_IDX, NIS_AC4_RH_MSK, NIS_AC4_RH_ON, isRearHeaterOn);
    ccSystem->setRearWindowHeating(isRearHeaterOn);
    break;
  }
}

