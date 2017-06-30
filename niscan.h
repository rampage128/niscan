#ifndef NISCAN_H
#define NISCAN_H

#include "binarydata.h"
#include "carduino.h"

class NissanClimateControlCanConnector : public CarSystemCanConnector {
  public:
    NissanClimateControlCanConnector(CarSystem* systemInstance) : CarSystemCanConnector(systemInstance), _can1(8), _can2(8), _can3(8) {
      _can1.writeByte(0, 0x20);
      _can1.writeByte(1, 0x64);
    };
    virtual void readCan(CanPacket* packet);
    virtual void writeCan(MCP_CAN* mcp);
    void pressModeButton();
    void pressWindshieldButton();
    void pressRearHeaterButton();
    void pressRecirculationButton();
    void pressAcButton();
    void pressOffButton();
    void pressAutoButton();
    void setTemperature(uint8_t temperature);
    void setFanSpeed(uint8_t fanSpeed);
  private:
    BinaryData _can1;
    BinaryData _can2;
    BinaryData _can3;
    uint8_t _roll = 0;
};

class NissanGearBoxCanConnector : public CarSystemCanConnector {
  public:
    NissanGearBoxCanConnector(CarSystem* systemInstance) : CarSystemCanConnector(systemInstance) {};
    virtual void readCan(CanPacket* packet);
};

class NissanCarConnector : public CarConnector {
  public:
    NissanCarConnector(uint8_t canInterruptPin, uint8_t canCsPin) : CarConnector(canInterruptPin, canCsPin) {
      _climateControl = new ClimateControl();
      _climateControlConnector = new NissanClimateControlCanConnector(_climateControl);
      _gearBox = new GearBox();
      _gearBoxConnector = new NissanGearBoxCanConnector(_gearBox);
    }
    ~NissanCarConnector() {
      delete _climateControl;
      delete _climateControlConnector;
      delete _gearBox;
      delete _gearBoxConnector;
    }
    virtual void updateFromSerial(BinaryBuffer* serialData);
  protected:
    virtual void updateFromCan(CanPacket* packet);
    virtual void broadcast(MCP_CAN* can);
  private:
    ClimateControl* _climateControl;
    NissanClimateControlCanConnector* _climateControlConnector;
    GearBox* _gearBox;
    NissanGearBoxCanConnector* _gearBoxConnector;
};

#endif

