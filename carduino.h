#ifndef CARSYSTEM_H
#define CARSYSTEM_H

#include "binarydata.h"
#include "canpacket.h"

class CarSystem {
  protected:
    uint8_t _id;
    BinaryData* _state;
  public:
    CarSystem(uint8_t id, uint8_t len){
      _id = id;
      _state = new BinaryData(len);
    };
    virtual ~CarSystem() {
      free(_state);
    }
    void serialize();
};

////////////////////////////////////////////

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

class CarSystemCanConnector {
  protected:
    CarSystem* _system;
  public:
    CarSystemCanConnector(CarSystem* systemInstance) { _system = systemInstance; }
    virtual void readCan(CanPacket* packet) = 0;
    virtual void writeCan(MCP_CAN* mcp) {};
};

#endif
