#include "network.h"
#include "canpacket.h"
#include "carduino.h"
#include "niscan_dfs.h"
#include "niscan.h"

NissanCarConnector carConnector(2, 10);

bool sniff = false;

void setup() {
  Serial.begin(115200);

  if (carConnector.setup()) {
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
  carConnector.update();
}

int packetStartIndex = -1;
int packetLength = -1;
BinaryBuffer serialBuffer(16);

void serialEvent() {
  while (Serial.available() && packetLength == -1 && serialBuffer.available() > 0) {
    byte singleData = Serial.read();
    serialBuffer.write(singleData);

    if (packetStartIndex == -1 && singleData == 0x7b) {
      packetStartIndex = serialBuffer.getPosition();
      continue;
    }

    if (singleData == 0x7d) {
      packetLength = serialBuffer.getPosition() - packetStartIndex + 1;
      break;
    }
  }

  if (packetStartIndex > -1 && packetLength > 0) {
    serialBuffer.goTo(packetStartIndex);
    unsigned char packetType = serialBuffer.readByte().data;
    switch (packetType) {
      case 0x61: {
        unsigned char command = serialBuffer.readByte().data;
        switch (command) {
          case 0x0a: // start sniffer
            carConnector.startSniffer();
            break;
          case 0x0b: // stop sniffer
            carConnector.stopSniffer();
            break;
          case 0x72: // set baud rate
            serialBuffer.next();
            BinaryData::LongResult result = serialBuffer.readLong();
            if (result.state == BinaryData::OK) {
              setBaudRate(result.data);
            }
          break;
        }
        break;
      }
      case 0x63:
        carConnector.updateFromSerial(&serialBuffer);
        break;
    }

    packetStartIndex = -1;
    packetLength = -1;
    serialBuffer.goTo(0);
  }

  if (!serialBuffer.available()) {
    serialBuffer.goTo(0);
  }
}

void setBaudRate(unsigned long newBaudRate) {
  unsigned long responseBaudRate = htonl(newBaudRate);
  Serial.write("{a");
  Serial.write(0x02);
  Serial.write(sizeof(responseBaudRate));
  Serial.write((byte*)&responseBaudRate, sizeof(responseBaudRate));
  Serial.write("}");
  Serial.flush();
  Serial.end();
  Serial.begin(newBaudRate);
  while (Serial.available()) {
    Serial.read();
  }
}

