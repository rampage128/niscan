#include "niscan.h"
#include "niscan_dfs.h"

template<> CarSystemPopulator<ClimateControl>::CarSystemPopulator(ClimateControl* sys) {
  _system = sys;
}

template<> void NissanClimateControlPopulator::populate(CanPacket* packet) {
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
    _system->setDesiredTemperature(temp);
    break;
    case (NIS_AC2_IDX):
    packet->readByte(NIS_AC2_MODE_IDX, mode);
    packet->checkFlag(NIS_AC2_WIND_IDX, NIS_AC2_WIND_MSK, NIS_AC2_WIND_ON, isWindshieldOn);
    packet->checkFlag(NIS_AC2_RECI_IDX, NIS_AC2_RECI_MSK, NIS_AC2_RECI_ON, isReciOn);
    packet->checkFlag(NIS_AC2_AUTO_IDX, NIS_AC2_AUTO_MSK, NIS_AC2_AUTO_ON, isAutomatic);
    packet->readByte(NIS_AC2_FANS_IDX, fans);
    switch (mode) {
      case (NIS_AC2_MODE_OFF):
      _system->setAirductWindshield(false);
      _system->setAirductFace(false);
      _system->setAirductFeet(false);
      break;
      case (NIS_AC2_MODE_FEET):
      _system->setAirductWindshield(false);
      _system->setAirductFace(false);
      _system->setAirductFeet(true);
      break;
      case (NIS_AC2_MODE_WINDFEET):
      _system->setAirductWindshield(true);
      _system->setAirductFace(false);
      _system->setAirductFeet(true);
      break;
      case (NIS_AC2_MODE_FACE):
      _system->setAirductWindshield(false);
      _system->setAirductFace(true);
      _system->setAirductFeet(false);
      break;
      case (NIS_AC2_MODE_FACEFEET):
      _system->setAirductWindshield(false);
      _system->setAirductFace(true);
      _system->setAirductFeet(true);
      break;
    }
    _system->setWindshieldHeating(isWindshieldOn);
    _system->setRecirculation(isReciOn);
    _system->setAuto(isAutomatic);
    _system->setFanLevel((fans - 4) / 8);
    break;
    case (NIS_AC3_IDX):
    packet->checkFlag(NIS_AC3_AC_IDX, NIS_AC3_AC_MSK, NIS_AC3_AC_ON, isAcOn);
    _system->setAc(isAcOn);
    break;
    case (NIS_AC4_IDX):
    packet->checkFlag(NIS_AC4_RH_IDX, NIS_AC4_RH_MSK, NIS_AC4_RH_ON, isRearHeaterOn);
    _system->setRearWindowHeating(isRearHeaterOn);
    break;
  }
}

