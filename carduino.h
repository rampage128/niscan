#ifndef CARSYSTEM_H
#define CARSYSTEM_H

#include "binarydata.h"
#include "canpacket.h"

class CarSystem {
  protected:
    uint8_t _id;
    BinaryData* _data;
  public:
    CarSystem(uint8_t id, uint8_t len){
      _id = id;
      _data = new BinaryData(len);
    };
    ~CarSystem() {
      free(_data);
    }
    void serialize();
};

class ClimateControl : public CarSystem {
  public:
    ClimateControl() :  CarSystem(0x24, 3) {}
    void setAc(bool state);
    void setAuto(bool state);
    void setAirductWindshield(bool state);
    void setAirductFace(bool state);
    void setAirductFeet(bool state);
    void setWindshieldHeating(bool state);
    void setRearWindowHeating(bool state);
    void setRecirculation(bool state);
    void setFanLevel(uint8_t fans);
    void setDesiredTemperature(uint8_t temperature);
};

class GearBox : public CarSystem {
  public:
    GearBox() :  CarSystem(0x67, 2) {}
    void setGear(int8_t gear);
    void setSynchroRev(bool state);
};

////////////////////////////////////////////////////////////////////////

class CarSystemPopulator {
  protected:
    CarSystem* _system;
  public:
    CarSystemPopulator(CarSystem* systemInstance) { _system = systemInstance; }
    virtual void populate(CanPacket* packet) = 0;
};

#endif
