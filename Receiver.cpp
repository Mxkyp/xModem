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
    const int n = 128;
    char szBuff[n + 1] = {0};
    DWORD dwBytesRead = 0;

    while(dwBytesRead != 0 && szBuff[0] != EOT) {
        if (!ReadFile(hSerial, szBuff, n, &dwBytesRead, NULL)) {
            //error occurred. Report to user.
        }
        sendControlSymbol(ACK);


        file.write(szBuff, dwBytesRead);
        file.flush();
    }

    // Print the received data
    std::cout << "Received: " << szBuff << std::endl;
    return true;
}
