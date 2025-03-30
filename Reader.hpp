//
// Created by mikol on 03/29/25.
//

#ifndef XMODEM_READER_HPP
#define XMODEM_READER_HPP

#include <Windows.h>
#include <iostream>
#include <fstream>
class Reader{

private:
    HANDLE hSerial;
    std::fstream file;
    std::string portName;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

public:
    Reader(std::string portName, std::string fileName);
    ~Reader();
    void openPort();
    void setTransmissionParams();
    void setTimeOuts();
    bool readPort();
    void writePort();
    void closePort();

    void initTransmission();

    void sendControlSign(int sign);

    int readControlSigns();

    void waitForNak();

    void transmit();
};

#endif //XMODEM_READER_HPP
