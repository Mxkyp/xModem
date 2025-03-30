#include <cstdint>

class xmodemPacket {
private:
  unsigned char SOH = SOH;
  unsigned char PacketNr;
  unsigned char data[128];
  unsigned char PacketNrFill;
};

class xmodemPacketCRC : xmodemPacket {
private:
  uint8_t CRC[2];
};

class xmodemPacketSum : xmodemPacket {
private:
  uint8_t checkSum;
};
