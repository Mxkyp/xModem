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
    bool readPort();
    int calculateChecksum(char *message);
    void getFrom(char* packet, char* message);
};


#endif //XMODEM_RECEIVER_H
