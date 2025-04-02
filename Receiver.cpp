#include "Receiver.hpp"
#include <vector>

Receiver::Receiver(std::string portName, std::string fileName) {
    this->portName = portName;
    this->file.open(fileName, std::ios_base::out | std::ios_base::binary);
};

void Receiver::initTransmission(){
    for (int i = 0; i < 6; ++i) {
        if(packetByteSize == 132) {
            sendControlSymbol(NAK);
        } else if(packetByteSize == 133) {
            sendControlSymbol('C');
        }
        std::cout << "Sending NAK (init transmission)";
        std::this_thread::sleep_for(std::chrono::seconds(10));
        while(readPort()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
}

bool Receiver::readPort() {
    auto* packet = new unsigned char[packetByteSize];
    DWORD dwBytesRead = 0;
    int messageLength = 0;
    int sum = 0;

    if (!ReadFile(hSerial, packet, packetByteSize, &dwBytesRead, NULL)) {
        //error occurred. Report to user.
    }

    unsigned char message[128] = {0};
    if(packet[0] == SOH) {
        getFrom(packet, message);
        for(unsigned char i : message) {
           sum += i;
           messageLength++;
        }
        if(calculateChecksum(message) == packet[packetByteSize - 1]) {
            sendControlSymbol(ACK);
            file.write(reinterpret_cast<const char *>(message), messageLength);
            file.flush();
        } else {
            std::cout << "in packet" << packet[packetByteSize - 1]<< std::endl;
            sendControlSymbol(NAK);
        }
    }
    else if (packet[0] == EOT) {
        return false;
    }

    return true;
}

void Receiver::getFrom(unsigned char* packet, unsigned char* message) {
    const int startIndex = 3;
    for (int i = 0; i < 128; i++) {
       message[i] = packet[i + startIndex];
    }
}

unsigned char Receiver::calculateChecksum(unsigned char* message) {
    int sum = 0;

    for(int i = 0; i < 128; i++) {
        sum += message[i];
    }

    std::cout << sum % 0x100 << std::endl;
    return static_cast<unsigned char>(sum % 0x100);
}
