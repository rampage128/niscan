#include <everytime.h>

#include <mcp_can.h>
#include <SPI.h>
#include "canpacket.h"
#include "carduino.h"
#include "niscan_dfs.h"
#include "niscan.h"

#define CLI_EVENT1_MODE B00010000
#define CLI_EVENT1_WINDSHIELD B00000001
#define CLI_EVENT2_REARHEATER B10000000
#define RAD_EVENT2_RECIRCULATION B00000010
#define RAD_EVENT3_TEMPERATURE B10000000
#define RAD_EVENT3_AC B00000100
#define RAD_EVENT3_OFF B00000001
#define RAD_EVENT4_AUTO B10000000
#define RAD_EVENT4_FAN B00010000

#define CLI_1_MASTER 1 << 0

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128]; // Array to store serial string

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

unsigned char temp = 0;
bool isAcOn = false;
unsigned char mode;
bool isWindshieldOn = false;
bool isReciOn = false;
unsigned char fans;

char modeString[128];

void loop() {
  // If CAN0_INT pin is low, read receive buffer
  if (!digitalRead(CAN0_INT)) { 
    CanPacket package = CanPacket::fromMcp(CAN0);
    climateControlPopulator.populate(&package);
  }

  every(500) {
    climate.serialize();
    //printStatus();
  }
}

void printStatus() {
    switch (mode) {
      case (NIS_AC2_MODE_OFF):
      sprintf(modeString, "%s", "off");
      break;
      case (NIS_AC2_MODE_FEET):
      sprintf(modeString, "%s", "feet");
      break;
      case (NIS_AC2_MODE_WINDFEET):
      sprintf(modeString, "%s", "windshield/feet");
      break;
      case (NIS_AC2_MODE_FACE):
      sprintf(modeString, "%s", "face");
      break;
      case (NIS_AC2_MODE_FACEFEET):
      sprintf(modeString, "%s", "face/feet");
      break;
    }
    
    sprintf(msgString, "Temp: %d°C, mode: %s, Windshield: %s, Air: %s, A/C: %s, Fan: %d", temp / 2, modeString, isWindshieldOn ? "on" : "off", isReciOn ? "recirculated" : "fresh", isAcOn ? "on" : "off", (fans - 4) / 8);
    Serial.println(msgString);
}

