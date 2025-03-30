#include <cstring>
#include "Sender.hpp"

//init the portName and name of the file to send
Sender::Sender(std::string portName, std::string fileName) :
Transmitter(portName, fileName, std::ios::in | std::ios::binary) {};

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

    if (mode == CHECKSUM) {
        n = 132;
    } else if (mode == CRC) {
        n = 133;
    }

    char *packet = new char[n];

    prepare(packet);
    std::cout << packet << std::endl;
    if (waitForSymbol() == NAK) {
        sendPacket(packet, n, dwBytesWritten);
    }
}

void Sender::prepare(char *packet) {
    int sum = 0;
    char c;
    char message[128] = {0};
    packet[0] = SOH;
    packet[1] = this->counter++ % 0xFF;
    packet[2] = 0xFF - this->counter;

    for(int i = 0; file.get(c) && i < 128; i++) {
        message[i] = c;
        sum += c;
    }

    std::cout << message << std::endl;
    memcpy(packet + 3, message, 128);

    packet[packetByteSize -1] = sum % 0xFF;
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
