#include <everytime.h>

#include <mcp_can.h>
#include <SPI.h>
#include "canpacket.h"
#include "carduino.h"
#include "niscan_dfs.h"
#include "niscan.h"

#define CAN0_INT 2
MCP_CAN CAN0(10);

ClimateControl climate;
NissanClimateControlPopulator climateControlPopulator(&climate);

GearBox gearBox;
NissanGearBoxPopulator gearBoxPopulator(&gearBox);

void setup() {
  Serial.begin(115200);

  // Initialize MCP2515 module
  if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    CAN0.setMode(MCP_NORMAL);                     
    pinMode(CAN0_INT, INPUT);
    Serial.write(">ardy|");
  }
  // Notify serial on error
  else {
    Serial.write(">e");
    Serial.write(0x01);
    Serial.write("|");
  }

  // Make sure we sent all serial data, before entering the loop
  Serial.flush();
}

void loop() {
  // Read can-bus data
  if (!digitalRead(CAN0_INT)) { 
    CanPacket package = CanPacket::fromMcp(CAN0);
    climateControlPopulator.populate(&package);
    gearBoxPopulator.populate(&package);
  }

  every(500) {
    climate.serialize();
  }
  every(1000) {
    gearBox.serialize();
  }
}

