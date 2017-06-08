#include <everytime.h>

#include <mcp_can.h>
#include <SPI.h>
#include "canpacket.h"
#include "carduino.h"
#include "niscan_dfs.h"
#include "niscan.h"

#define CAN0_INT 2  // Set INT to pin 2
MCP_CAN CAN0(10);   // Set CS to pin 10

ClimateControl climate;
NissanClimateControlPopulator climateControlPopulator(&climate);

void setup() {
  Serial.begin(115200);
  
  // Initialize MCP2515 running at 8MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("MCP2515 Initialized Successfully!");
  }
  else {
    Serial.println("Error Initializing MCP2515...");
  }

  // Set operation mode to normal so the MCP2515 sends acks to received data.
  CAN0.setMode(MCP_NORMAL);                     

  // Configuring pin for /INT input
  pinMode(CAN0_INT, INPUT);                            
  
  Serial.println("Listening...");
}

void loop() {
  // If CAN0_INT pin is low, read receive buffer
  if (!digitalRead(CAN0_INT)) { 
    CanPacket package = CanPacket::fromMcp(CAN0);
    climateControlPopulator.populate(&package);
  }

  every(500) {
    climate.serialize();
  }
}

