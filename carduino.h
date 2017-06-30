#ifndef CARSYSTEM_H
#define CARSYSTEM_H

#include "binarydata.h"
#include "canpacket.h"
#include <SPI.h>
#include <mcp_can.h>
#include <everytime.h>

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
      delete _state;
    }
    void serialize();
};

////////////////////////////////////////////

class ClimateControl : public CarSystem {
  public:
    ClimateControl() :  CarSystem(0x63, 3) {}
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

////////////////////////////////////////////////////////////////////////

class CarConnector {
  private:
    uint8_t _canInterruptPin = 2;
    boolean _isInitialized = false;
    MCP_CAN* _can;
    boolean _sniff = false;
  public:
    CarConnector(uint8_t canInterruptPin, uint8_t canCsPin) {
      _can = new MCP_CAN(canCsPin);
      _canInterruptPin = canInterruptPin;
    }
    virtual ~CarConnector() {
      delete _can;
    }
    boolean setup() {
      uint8_t canStatus = _can->begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ);
      if (canStatus == CAN_OK) {
        _can->setMode(MCP_NORMAL);
        pinMode(_canInterruptPin, INPUT);
        _isInitialized = true;
      }
      return _isInitialized;
    }
    void update() {
      if (!_isInitialized) {
        return;
      }
      if (!digitalRead(_canInterruptPin)) {
          CanPacket* packet = CanPacket::fromMcp(_can);
          updateFromCan(packet);
          if (_sniff) {
            BinaryData* data = packet->getData();
            uint8_t packetSize = data->getSize();

            unsigned long int packetId = htonl(packet->getId());
            int packetIdLenght = sizeof(packetId);

            Serial.write("{b");
            Serial.write(0x6d);
            Serial.write(packetSize + packetIdLenght);
            Serial.write((byte*)&packetId, packetIdLenght);
            for (int i = 0; i < packetSize; i++) {
              Serial.write(data->readByte(i).data);
            }
            Serial.write("}");
          }
          delete packet;
      }

      if (!_sniff) {
        broadcast(_can);
      }
    }
    void startSniffer() {
      _sniff = true;
    }
    void stopSniffer() {
      _sniff = false;
    }
  protected:
    virtual void updateFromSerial(BinaryBuffer* serialData) = 0;
    virtual void updateFromCan(CanPacket* packet) = 0;
    virtual void broadcast(MCP_CAN* can) = 0;
};

#endif
