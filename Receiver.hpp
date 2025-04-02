//
// Created by mikol on 03/30/25.
//

#ifndef XMODEM_RECEIVER_H
#define XMODEM_RECEIVER_H
#include "Transmitter.hpp"

class Receiver : public Transmitter {
public:
    Receiver(std::string portName, std::string fileName);
    ~Receiver();
    void initTransmission();
    unsigned char readPort();
    void getFrom(unsigned char *packet, unsigned char *message);
    unsigned char calculateChecksum(unsigned char *message);


    void processCS(unsigned char *packet, unsigned char *message, int messageLength);
    void processCRC(unsigned char *packet, unsigned char *message, int messageLength);
};


#endif //XMODEM_RECEIVER_H
