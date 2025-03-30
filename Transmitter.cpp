//
// Created by mikol on 03/29/25.
//
#include "Transmitter.hpp"
#include "instructions.hpp"
#include <fstream>
#include <chrono>
#include <thread>

Transmitter::Transmitter(std::string portName, std::string fileName)
    : portName(portName), file(fileName, std::ios::out | std::ios::binary) {
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

void Transmitter::initTransmission(){
    for (int i = 0; i < 6; ++i) {
        sendControlSymbol(NAK);
        std::cout << "Sending NAK (init transmission)";
        std::this_thread::sleep_for(std::chrono::seconds(10));
        if(readPort()) {
            break;
        }
        std::cout << "no response";
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


bool Transmitter::readPort() {
    const int n = 128;
    char szBuff[n + 1] = {0};
    DWORD dwBytesRead = 0;
    if(!ReadFile(hSerial, szBuff, n, &dwBytesRead, NULL)){
       //error occurred. Report to user.
    }

    if(dwBytesRead == 0 || szBuff[0] == EOT) {
        return false;
    }

    file.write(szBuff, dwBytesRead);
    file.flush();

    // Print the received data
    std::cout << "Received: " << szBuff << std::endl;
    return true;
}



unsigned char Transmitter::waitForSymbol() {
    unsigned char sign = readControlSymbol();
    while(sign != NAK && sign != ACK) {
        sign = readControlSymbol();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return sign;
}

unsigned char Transmitter::readControlSymbol() {
    unsigned char szBuff = -1;
    DWORD dwBytesRead = 0;
    if(!ReadFile(hSerial, &szBuff, 1, &dwBytesRead, NULL)){
        //error occurred. Report to user.
    }
    return szBuff;
}

void Transmitter::writePort() {
    waitForSymbol();
    const int n = 128;
    char szBuff[n + 1] = {0};  // +1 to ensure there's space for the null terminator
    DWORD dwBytesWritten = 0;

    // Populate the buffer with your data to be sent
    // For now, you might want to test sending something simple like "Hello"
    strncpy(szBuff, "Hello1", n);

    if (!WriteFile(hSerial, szBuff, strlen(szBuff), &dwBytesWritten, NULL)) {
        // Error occurred, report to the user
        DWORD dwError = GetLastError();
        std::cerr << "WriteFile failed with error " << dwError << std::endl;
        return;
    }

    std::cout << "Sent: " << szBuff << std::endl;
}

void Transmitter::closePort(void){
    CloseHandle(hSerial);
}

Transmitter::~Transmitter() {
    file.close();
    CloseHandle(hSerial);
}
