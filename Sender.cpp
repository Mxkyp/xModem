#include <cstring>
#include "Sender.hpp"

//init the portName and name of the file to send
Sender::Sender(std::string portName, std::string fileName) : Transmitter(portName, fileName, std::ios::in | std::ios::binary) {};

//wait for either ACK or NAK
unsigned char Sender::waitForSymbol() {
    unsigned char sign = readControlSymbol();
    while(sign != NAK && sign != ACK) {
        sign = readControlSymbol();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return sign;
}

void Sender::writePort() {
    int n = 0;
    DWORD dwBytesWritten = 0;

    if(mode == CHECKSUM) {
       n = 132;
    } else if(mode == CRC) {
        n = 133;
    }

    char* szBuff = new char[n];
    char* lastSent = new char[n];
    file.read(szBuff, n);
    memcpy(lastSent, szBuff, n);

    do {
       if(waitForSymbol() == NAK)  {
           sendPacket(lastSent, n, dwBytesWritten);
       } else {
           memcpy(lastSent, szBuff, n);
           sendPacket(szBuff, n, dwBytesWritten);
       }
    } while (file.read(szBuff, n));

    delete[] lastSent;
}

void Sender::sendPacket(char *packet,int n, DWORD bytesWritten) {
    if (!WriteFile(hSerial, packet, n, &bytesWritten, NULL)) {
        // Error occurred, report to the user
        DWORD dwError = GetLastError();
        std::cerr << "WriteFile failed with error " << dwError << std::endl;
        return;
    }
}

Sender::~Sender() {
    delete szBuff;
}
