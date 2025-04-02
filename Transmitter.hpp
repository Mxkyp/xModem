//
// Created by mikol on 03/29/25.
//

#ifndef XMODEM_TRANSMITTER_HPP
#define XMODEM_TRANSMITTER_HPP
#include "instructions.hpp"

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <string>

#define CRC 0
#define CHECKSUM 1
class Transmitter{

protected:
    HANDLE hSerial;
    std::fstream file;
    std::string portName;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    boolean mode = CHECKSUM;
    int packetByteSize = 133;

public:
    ~Transmitter();
    void openPort();
    void setTransmissionParams();
    void setTimeOuts();
    void sendControlSymbol(unsigned char Symbol);
    unsigned char readControlSymbol();

    int calcrc(char *ptr, int count);

    uint16_t calcrc(unsigned char *ptr, uint16_t count);
};

#endif //XMODEM_TRANSMITTER_HPP
