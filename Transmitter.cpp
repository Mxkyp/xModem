//
// Created by mikol on 03/29/25.
//
#include "Transmitter.hpp"

Transmitter::Transmitter(std::string portName, std::string fileName, std::ios_base::openmode flags)
    : portName(portName), file(fileName, flags) {
    std::cout << "flags " << fileName << std::endl;
    if (!file) {
        std::cerr << "Error opening file: " << fileName << std::endl;
    }
}

void Transmitter::openPort(void) {
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
    setTransmissionParams();
    setTimeOuts();
}

 void Transmitter::setTransmissionParams(void) {
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

void Transmitter::setTimeOuts(void){
    timeouts.ReadIntervalTimeout=50;
    timeouts.ReadTotalTimeoutConstant=50;
    timeouts.ReadTotalTimeoutMultiplier=10;
    timeouts.WriteTotalTimeoutConstant=50;
    timeouts.WriteTotalTimeoutMultiplier=10;
    if(!SetCommTimeouts(hSerial, &timeouts)){
        //error occureed. Inform user
    }
}


void Transmitter::sendControlSymbol(unsigned char Symbol) {
    DWORD dwBytesWritten = 0;
    if (!WriteFile(hSerial, &Symbol, 1, &dwBytesWritten, NULL)) {
        // Error occurred, report to the user
        DWORD dwError = GetLastError();
        std::cerr << "WriteFile failed with error " << dwError << std::endl;
        return;
    }
}




unsigned char Transmitter::readControlSymbol() {
    unsigned char szBuff = -1;
    DWORD dwBytesRead = 0;
    if(!ReadFile(hSerial, &szBuff, 1, &dwBytesRead, NULL)){
        //error occurred. Report to user.
    }
    return szBuff;
}


void Transmitter::closePort(void){
    CloseHandle(hSerial);
}

Transmitter::~Transmitter() {
    file.close();
    CloseHandle(hSerial);
}
