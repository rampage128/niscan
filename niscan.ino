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
NissanClimateControlCanConnector climateControlCanConnector(&climate);

GearBox gearBox;
NissanGearBoxCanConnector gearBoxCanConnector(&gearBox);

void setup() {
  Serial.begin(115200);

  // Initialize MCP2515 module
  if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    CAN0.setMode(MCP_NORMAL);                     
    pinMode(CAN0_INT, INPUT);
    Serial.write("{a");
    Serial.write(0x01);
    Serial.write("}");
  }
  // Notify serial on error
  else {
    Serial.write("{e");
    Serial.write(0x01);
    Serial.write("}");
  }

  // Make sure we sent all serial data, before entering the loop
  Serial.flush();
}

void loop() {
  // Read can-bus data
  if (!digitalRead(CAN0_INT)) { 
    CanPacket packet = CanPacket::fromMcp(&CAN0);
    climateControlCanConnector.readCan(&packet);
    gearBoxCanConnector.readCan(&packet);
  }

  every(250) {
    climate.serialize();
    climateControlCanConnector.writeCan(&CAN0);
  }
  every(1000) {
    gearBox.serialize();
  }
}


int readIndex = -1;
int packetStartIndex = -1;
int packetEndIndex = -1;
byte data[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void serialEvent(){
  while(Serial.available() && packetEndIndex == -1 && readIndex < 16) {
    data[++readIndex] = Serial.read();
    byte singleData = data[readIndex];

    if (packetStartIndex == -1 && singleData == 0x7b) {
      packetStartIndex = readIndex;
      continue;
    }

    if (singleData == 0x7d) {
      packetEndIndex = readIndex;
      continue;
    }
  }

  if (packetStartIndex > -1 && packetEndIndex > packetStartIndex) {
    if (data[packetStartIndex+1] == 0x63) {
      switch (data[packetStartIndex+2]) {
        case 0x01: // OFF BUTTON
          climateControlCanConnector.pressOffButton();
          break;
        case 0x02: // ac button
          climateControlCanConnector.pressAcButton();
          break;
        case 0x03: // auto button
          climateControlCanConnector.pressAutoButton();
          break;
        case 0x04: // recirculation button
          climateControlCanConnector.pressRecirculationButton();
          break;
        case 0x05: // windshield heating button
          climateControlCanConnector.pressWindshieldButton();
          break;
        case 0x06: // rear window heating button
          climateControlCanConnector.pressRearHeaterButton();
          break;
        case 0x07: // mode button
          climateControlCanConnector.pressModeButton();
          break;
        case 0x08: // temperature knob
          climateControlCanConnector.setTemperature(data[packetStartIndex+4]);
          break;
        case 0x09: // fan level
          climateControlCanConnector.setFanSpeed(data[packetStartIndex+4]);
          break;
      }
    }

    readIndex = -1;
    packetStartIndex = -1;
    packetEndIndex = -1;
    for (int i = 0; i < 16; i++) {
      data[i] = 0x00;
    }
  }
}

