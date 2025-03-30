#include "instructions.hpp"
#include <iostream>
#include "Reader.hpp"
int main(int argc, char *argv[]) {
    /*
  printf("\n[1] XModem");
  printf("\n[2] XModem(CRC)\n");
  printf("\n[3] exit\n");
     */

    char received;

    printf("\n[1] receive");
    printf("\n[2] send\n");
    printf("\n[3] exit\n");

  while (received != '1' && received != '2' && received != '3') {
    std::cin.get(received);
  }

  if(received == '1'){
      auto *reader = new Reader("COM1", "file.txt");
      reader->openPort();
      reader->readPort();
  }

  if(received == '2'){
        auto *reader = new Reader("COM2", "file2.txt");
        reader->openPort();
        reader->writePort();
  }

  return 0;
}
