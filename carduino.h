#ifndef CARDATA_H
#define CARDATA_H

#include "canpacket.h"

class CarData {
  public:
    CarData();
    CarData(uint8_t len);
    typedef enum STATUS {
      OK,
      INDEXOUTOFBOUNDS
    } AccessStatus;
    AccessStatus setFlag(uint8_t index, unsigned char mask, bool value);
    AccessStatus setByte(uint8_t index, unsigned char value);
    AccessStatus readFlag(uint8_t index, unsigned char mask, bool &result);
    AccessStatus readByte(uint8_t index, unsigned char &buf);
    char* serialize();
    uint8_t getSize();
  private:
    uint8_t _len = 0;
    char* _payload;
    AccessStatus checkIndex(uint8_t index);
};

#endif

#ifndef CARSYSTEM_H
#define CARSYSTEM_H

class CarSystem {
  protected:
    uint8_t _id;
    CarData* _data;
  public:
    CarSystem(uint8_t id, uint8_t len);
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
