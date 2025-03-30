//
// Created by mikol on 03/29/25.
//

#ifndef XMODEM_READER_H
#define XMODEM_READER_H

#include <Windows.h>
#include <iostream>
class Reader{

private:
    HANDLE hSerial;
    std::string portName;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

public:
    explicit Reader(std::string portName);
    void openPort(void);
    void setTransmissionParams(void);
    void setTimeOuts(void);
    void readPort(void);
    void writePort(void);
    void closePort(void);
};

#endif //XMODEM_READER_H
