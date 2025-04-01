#include "Receiver.hpp"

Receiver::Receiver(std::string portName, std::string fileName) : Transmitter(portName, fileName) {};

// send NAK every ten seconds for 1 minute if you get a response read it
void Receiver::initTransmission(){
    for (int i = 0; i < 6; ++i) {
        sendControlSymbol(NAK);
        std::cout << "Sending NAK (init transmission)";
        std::this_thread::sleep_for(std::chrono::seconds(10));
        while(readPort()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(90));
        }
        break;
        std::cout << "no response";
    }
}

bool Receiver::readPort() {
    char* packet = new char[packetByteSize];
    DWORD dwBytesRead = 0;
    int messageLength = 0;
    int sum = 0;

    if (!ReadFile(hSerial, packet, packetByteSize, &dwBytesRead, NULL)) {
        //error occurred. Report to user.
    }

    char message[128] = {0};
    if(packet[0] == SOH) {
        getFrom(packet, message);
        for(char i : message) {
            if(i == '\0') {
                break;
            }
           sum += i;
           messageLength++;
        }
        if(calculateChecksum(message) == packet[packetByteSize - 1]) {
            sendControlSymbol(ACK);
            std::cout << "checksum good";
        } else {
            std::cout << "in packet" << packet[packetByteSize - 1]<< std::endl;
            sendControlSymbol(NAK);
        }
    }
    else if (packet[0] == EOT) {
        return false;
    }

    file.write(message, messageLength);
    file.flush();
    return true;
}

void Receiver::getFrom(char* packet, char* message) {
    const int startIndex = 3;
    for (int i = 0; i < 128; i++) {
       message[i] = packet[i + startIndex];
    }
}

int Receiver::calculateChecksum(char* message) {
    int sum = 0;

    for(int i = 0; i < 128; i++) {
        sum += message[i];
    }

    std::cout << sum % 0xFF << std::endl;
    return sum % 0xFF;
}
