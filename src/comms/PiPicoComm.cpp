#include "PiPicoComm.hpp"

size_t PiPicoComm::read(unsigned char* buf, const size_t count) {
    return uart.read_bytes(buf, count);
}

void PiPicoComm::write(unsigned char* buf, const size_t count) {
    uart.send_bytes(buf, count);
}
