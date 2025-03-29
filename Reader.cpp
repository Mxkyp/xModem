//
// Created by mikol on 03/29/25.
//
#include "Reader.h"
    void Reader::openPort(std::string portName) {
        hSerial = CreateFile(portName.c_str(),
                             GENERIC_READ | GENERIC_WRITE,
                             0,
                             0,
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL,
                             0);
        if (hSerial == INVALID_HANDLE_VALUE) {
            if (GetLastError() == ERROR_FILE_NOT_FOUND) {
                //serial port does not exist. Inform user.
            }
            //some other error occurred. Inform user.
        }
    }

     void Reader::setTransmissionParams() {
        dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
        if (!GetCommState(hSerial, &dcbSerialParams)) {
            //error getting state
        }
        dcbSerialParams.BaudRate=CBR_19200;
        dcbSerialParams.ByteSize=8;
        dcbSerialParams.StopBits=ONESTOPBIT;
        dcbSerialParams.Parity=NOPARITY;
        if(!SetCommState(hSerial, &dcbSerialParams)){
            //error setting serial port state
        }
    }

    void Reader::setTimeOuts(){
        timeouts.ReadIntervalTimeout=50;
        timeouts.ReadTotalTimeoutConstant=50;
        timeouts.ReadTotalTimeoutMultiplier=10;
        timeouts.WriteTotalTimeoutConstant=50;
        timeouts.WriteTotalTimeoutMultiplier=10;
        if(!SetCommTimeouts(hSerial, &timeouts)){
            //error occureed. Inform user
        }
    }

    void Reader::readPort(void) {
        const int n = 128;
        char szBuff[n + 1] = {0};
        DWORD dwBytesRead = 0;
        if(!ReadFile(hSerial, szBuff, n, &dwBytesRead, NULL)){
            //error occurred. Report to user.
        }

        for(int i = 0; i < n; i++) {
            std::cout << szBuff[i];
        }
    }

    void Reader::writePort(void) {
        const int n = 128;
        char szBuff[n + 1] = {0};
        DWORD dwBytesRead = 0;
        if(!ReadFile(hSerial, szBuff, n, &dwBytesRead, NULL)){
            //error occurred. Report to user.
            return;
        }

        for(int i = 0; i < n; i++) {
            std::cout << szBuff[i];
        }
    }

    void Reader::closePort(){
        CloseHandle(hSerial);
    }
};