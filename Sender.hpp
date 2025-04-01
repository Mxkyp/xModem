//
// Created by mikol on 03/30/25.
//

#ifndef XMODEM_SENDER_H
#define XMODEM_SENDER_H
#include "Transmitter.hpp"

class Sender : public Transmitter {
    unsigned char counter = 0x01;
    char *szBuff;

    public:
    Sender(std::string portName, std::string fileName);
    ~Sender();
    void writePort();
    unsigned char waitForSymbol();
    void transmit();

    void sendPacket(char *packet, int n, DWORD bytesWritten);

    void prepare(char *packet);

    void sendPacket(char *packet, DWORD bytesWritten);

    int setMessageGetSum(char *message, int *sum);
};


#endif //XMODEM_SENDER_H
