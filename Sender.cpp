#include "Sender.hpp"

Sender::Sender(std::string portName, std::string fileName) {
    this->portName = portName;
    this->file.open(fileName, std::ios_base::in | std::ios_base::binary);
}

//wait for either ACK or NAK
unsigned char Sender::waitForResponse() {
    unsigned char sign = readControlSymbol();
    while(sign != NAK && sign != ACK) {
        sign = readControlSymbol();
    }
    return sign;
}

unsigned char Sender::waitFor(unsigned char symbol) {
    unsigned char sign = readControlSymbol();
    while(sign != symbol) {
        sign = readControlSymbol();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Sender::writePort() {
    int n = 0;
    DWORD dwBytesWritten = 0;

    auto *packet = new unsigned char[packetByteSize];

    prepare(packet);
    waitForResponse();
    sendPacket(packet, dwBytesWritten);

    while(packet[0] != EOT){
        unsigned char response = waitForResponse();
        if(response == ACK) {
            prepare(packet);
        }
        sendPacket(packet, dwBytesWritten);
    }
}

void Sender::prepare(unsigned char *packet) {
    int sum = 0;
    unsigned char message[128] = {0};
    packet[0] = SOH;
    packet[1] = this->counter % 0x100;
    packet[2] = 0xFF - packet[1];
    this->counter++;

    int messLength = setMessageGetSum(message, &sum);
    if(packetByteSize == 132) {
        packet[packetByteSize -1] = static_cast<unsigned char>(sum % 0x100); //set checksum byte
    } else if (packetByteSize == 133) {
        uint16_t crc = calcrc(message, 128);
        packet[packetByteSize - 2] = static_cast<unsigned char> (crc / 256);
        packet[packetByteSize - 1] = static_cast<unsigned char> (crc % 256);
    }
    if(messLength == 0) {
        packet[0] = EOT; // if read nothing, mark end of transmission
    }
    memcpy(packet + 3, message, 128);

    std::cout << sum % 0x100 << std::endl;
}

int Sender::setMessageGetSum(unsigned char* message, int* sum) {
    int byte = 0;
    int messLength = 0;
    while (messLength < 128 && (byte = file.get()) != EOF) {
        message[messLength++] = static_cast<unsigned char>(byte);
        (*sum) += byte;
    }
    if(messLength > 0 && messLength < 128){
        while(messLength < 128){
            message[messLength] = 0x00;
            messLength++;
        }
    }

    return messLength;
}

void Sender::sendPacket(unsigned char *packet, DWORD bytesWritten) {
    if (!WriteFile(hSerial, packet, packetByteSize, &bytesWritten, NULL)) {
        // Error occurred, report to the user
        DWORD dwError = GetLastError();
        std::cerr << "WriteFile failed with error " << dwError << std::endl;
        return;
    }
}

Sender::~Sender() {
    delete szBuff;
}
