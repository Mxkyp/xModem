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

        unsigned char gotResponse;
        do {
            gotResponse = readPort();
        } while(gotResponse == ACK);
        if(gotResponse == EOT) {
            break;
        }
    }
}

 unsigned char Receiver::readPort() {
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
        for (unsigned char i: message) {
            sum += i;
            messageLength++;
        }
        if(packetByteSize == 132) {
            processCS(packet, message, messageLength);
        } else if (packetByteSize == 133) {
            processCRC(packet, message, messageLength);
        }
    }
    else if (packet[0] == EOT) {
        return EOT;
    } else if(messageLength == 0){
        return NAK;
    }

    return ACK;
}

void Receiver::processCS(unsigned char* packet, unsigned char* message, int messageLength) {
    if (calculateChecksum(message) == packet[packetByteSize - 1]) {
        sendControlSymbol(ACK);
        file.write(reinterpret_cast<const char *>(message), messageLength);
        file.flush();
    } else {
        std::cout << "in packet" << packet[packetByteSize - 1] << std::endl;
        sendControlSymbol(NAK);
    }
}

void Receiver::processCRC(unsigned char* packet, unsigned char* message, int messageLength) {
    uint16_t crc = calcrc(message, 128);
    unsigned char msb = packet[packetByteSize - 2];
    unsigned char lsb = packet[packetByteSize - 1];
    uint16_t crcFromPacket =  msb * 256 + lsb;
    if (crc == crcFromPacket) {
        sendControlSymbol(ACK);
        file.write(reinterpret_cast<const char *>(message), messageLength);
        file.flush();
    } else {
        std::cout << "in packet" << crcFromPacket << std::endl;
        std::cout << "actual" << crc << std::endl;
        sendControlSymbol(NAK);
    }
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
