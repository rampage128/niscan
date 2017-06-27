# Niscan

> CAN-Bus to USB-Serial interface for Arduino

This is an [Arduino (Nano)](https://www.arduino.cc/en/Main/ArduinoBoardNano) firmware to allow a 
serial device to connect to a car. It provides a translation layer between CAN-Bus of the car and 
USB-Port of another device.

Selected CAN-Bus packets are read from the car's CAN-Bus. Then the gathered data is grouped 
logicallly and translated into small USB-Serial binary packets.

__Please make sure to read the [requirements](#requirements)__!

## Background
Niscan is an interface to allow the app [CarDroid](https://github.com/rampage128/cardroid) to
communicate with a nissan 370z. Main goal was to replace the cars entertainment system with an 
Android device, while keeping the climate controls functional.

## Requirements

- [Arduino Nano](https://www.arduino.cc/en/Main/ArduinoBoardNano) or 
  [other Arduino board](https://www.arduino.cc/en/Main/Products#entrylevel) 
- A [MCP2515 SPI board](https://www.amazon.de/dp/B01IV3ZSKO/)
- __A Nissan 370z__ Currently Niscan only supports that car!  
  Other Nissan cars might have the same CAN-Bus layout and work with this project.

## Installation
### Releases

Currently no releases, you have to build the project [From source](#from-source)

### From source

1. Get your favourite [Arduino IDE](https://www.arduino.cc/en/main/software)
2. Clone the repository into your sketch directory  
   ```
   git clone https://github.com/rampage128/niscan.git
   ```
3. Compile and upload to your Arduino

## Usage

1. Install on your Arduino
2. Hook up your Arduino to the MCP2515 via SPI.
3. Connect to the CAN-Bus of your car (also possible via OBD-Port)

The Arduino sends and receives serial packets to the USB-Serial interface.

Easiest way to make real use of the data is to connect an Android device to the Arduino's USB-Port
and get [CarDroid](https://github.com/rampage128/cardroid).

The hard way is to read the packets from the serial connection. The protocol is custom and subject
to change. Generally data is sent in frames. Each frame contains one single packet. 


Please see the table below for a general idea:

| Index  | Length      | Possible values | Description                                  |
| ------:| -----------:| --------------- | -------------------------------------------- |
| 0      | 1           | `0x7b`          | Start of a frame                             |
| 1      | 1           | `0x00` - `0xFF` | Packet type to indicate purpose              |
| 2      | 1           | `0x00` - `0xFF` | Packet id to indicate request                |
| 3      | 1           | `0x01` - `0x7c` | Payload length (L) (only if payload present) |
| 4      | (1 - 124) L | `0x01` - `0xFF` | Payload (only if present)                    |
| 3 + L  | 1           | `0x7d`          | End of a frame                               |

For more information, please refer to the [source](https://github.com/rampage128/niscan).

## Contribute

Feel free to [open an issue](https://github.com/rampage128/niscan/issues) or submit a PR

Also, if you like this or other of my projects, please feel free to support me using the Link below.

[![Buy me a beer](https://img.shields.io/badge/buy%20me%20a%20beer-PayPal-green.svg)](https://www.paypal.me/FrederikWolter/1)

## Dependencies

- [everytime](https://github.com/fesselk/everytime) for easy periodic code execution (send packets)
- [MCP_CAN_lib](https://github.com/coryjfowler/MCP_CAN_lib) to communicate with CAN-Bus on MCP2515
- [SPI library](https://www.arduino.cc/en/Reference/SPI) to utilize SPI interface
