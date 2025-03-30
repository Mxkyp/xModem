//
// Created by mikol on 03/29/25.
//
#include "Reader.hpp"
#include "instructions.hpp"
#include <fstream>
#include <future>
#include <chrono>
#include <thread>

Reader::Reader(std::string portName, std::string fileName)
    : portName(portName), file(fileName, std::ios::out | std::ios::binary) {
    if (!file) {
        std::cerr << "Error opening file: " << fileName << std::endl;
    }
}

void Reader::openPort(void) {
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

 void Reader::setTransmissionParams(void) {
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

void Reader::setTimeOuts(void){
    timeouts.ReadIntervalTimeout=50;
    timeouts.ReadTotalTimeoutConstant=50;
    timeouts.ReadTotalTimeoutMultiplier=10;
    timeouts.WriteTotalTimeoutConstant=50;
    timeouts.WriteTotalTimeoutMultiplier=10;
    if(!SetCommTimeouts(hSerial, &timeouts)){
        //error occureed. Inform user
    }
}

void Reader::transmit(){
   initTransmission();
}

void Reader::initTransmission(){
    for (int i = 0; i < 6; ++i) {
        sendControlSign(NAK);
        std::this_thread::sleep_for(std::chrono::seconds(10));
        if(readPort()) {
            break;
        }
    }
}

void Reader::sendControlSign(int sign) {
    char signByte = static_cast<char>(sign); // Convert int to char
    DWORD dwBytesWritten = 0;
    if (!WriteFile(hSerial, &signByte, 1, &dwBytesWritten, NULL)) {
        // Error occurred, report to the user
        DWORD dwError = GetLastError();
        std::cerr << "WriteFile failed with error " << dwError << std::endl;
        return;
    }
}

void Reader::waitForNak() {
    int sign = readControlSigns();
    while(sign != NAK) {
        sign = readControlSigns();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int Reader::readControlSigns() {
    unsigned char szBuff = -1;
    DWORD dwBytesRead = 0;
    if(!ReadFile(hSerial, &szBuff, 1, &dwBytesRead, NULL)){
        //error occurred. Report to user.
    }
    return static_cast<int>(szBuff);
}

bool Reader::readPort() {
    const int n = 128;
    char szBuff[n + 1] = {0};
    DWORD dwBytesRead = 0;
    if(!ReadFile(hSerial, szBuff, n, &dwBytesRead, NULL)){
        //error occurred. Report to user.
    }

    if(dwBytesRead == 0) {
        return false;
    }

    szBuff[dwBytesRead] = '\0';

    file.write(szBuff, dwBytesRead);
    file.flush();

    // Print the received data
    std::cout << "Received: " << szBuff << std::endl;
    return true;
}

void Reader::writePort(void) {
    waitForNak();
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

void Reader::closePort(void){
    CloseHandle(hSerial);
}

Reader::~Reader() {
    file.close();
    CloseHandle(hSerial);
}
