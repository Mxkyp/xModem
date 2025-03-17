#include <cstdint>

class xmodemPacket {
private:
  uint8_t SOH;
  uint8_t PacketNr;
  uint8_t data[128];
  uint8_t PacketNrFill;
};

class xmodemPacketCRC : xmodemPacket {
private:
  uint8_t CRC[2];
};

class xmodemPacketSum : xmodemPacket {
private:
  uint8_t checkSum;
};
