//
// Created by mikol on 03/29/25.
//

#ifndef XMODEM_TRANSMITTER_HPP
#define XMODEM_TRANSMITTER_HPP

#include <Windows.h>
#include <iostream>
#include <fstream>
class Transmitter{

private:
    HANDLE hSerial;
    std::fstream file;
    std::string portName;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

public:
    //common
    Transmitter(std::string portName, std::string fileName);
    ~Transmitter();
    void openPort();
    void closePort();
    void setTransmissionParams();
    void setTimeOuts();
    void sendControlSymbol(unsigned char Symbol);
    int readControlSymbol();

    // receiver
    void initTransmission();
    bool readPort();

    // sender
    void writePort();
    void waitForNak();
    void transmit();
};

#endif //XMODEM_TRANSMITTER_HPP
