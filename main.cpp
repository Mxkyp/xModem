#include "instructions.hpp"
#include <iostream>
#include "Transmitter.hpp"
#include "Receiver.hpp"
#include "Sender.hpp"

int main(int argc, char *argv[]) {

    char received;

    printf("\n[1] receive");
    printf("\n[2] send\n");
    printf("\n[3] exit\n");

  while (received != '1' && received != '2' && received != '3') {
    std::cin.get(received);
  }
    //a.pdf
    //b.pdf
  if(received == '1'){
      auto *reader = new Receiver("COM1", "b.png");
      reader->openPort();
      reader->initTransmission();
  }

  if(received == '2'){
        auto *sender = new Sender("COM2", "a.png");
        sender->openPort();
        sender->writePort();
  }

  return 0;
}
