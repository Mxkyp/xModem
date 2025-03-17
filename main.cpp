#include "instructions.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  printf("\n[1] XModem");
  printf("\n[2] XModem(CRC)\n");
  printf("\n[3] exit\n");
  char received;

  while (received != '1' && received != '2' && received != '3') {
    std::cin.get(received);
    printf("\n[1] receive");
    printf("\n[2] send\n");
    printf("\n[3] exit\n");
  }

  return 0;
}
