#include "Receiver.hpp"

Receiver::Receiver(std::string portName, std::string fileName) : Transmitter(portName, fileName) {};

void Receiver::initTransmission(){
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

bool Receiver::readPort() {
    char* szBuff = new char[packetByteSize];
    DWORD dwBytesRead = 0;

    if (!ReadFile(hSerial, szBuff, packetByteSize, &dwBytesRead, NULL)) {
        //error occurred. Report to user.
    }
    char message[128] = {0};
    if(szBuff[0] == SOH) {
        memcpy(message, szBuff+3, 128);
    }


    std::cout << "Received: " << message << std::endl;

    file.write(message, 128);
    file.flush();

    // Print the received data
}
