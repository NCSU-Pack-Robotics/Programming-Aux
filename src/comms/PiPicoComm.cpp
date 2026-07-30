#include "PiPicoComm.hpp"

PiPicoComm::~PiPicoComm() {
    mutex_exit(&mutex);
}

size_t PiPicoComm::read(unsigned char* buf, const size_t count) {
    if (count == 0) return 0;

    const bool read_success = uart.read_bytes(buf, count);
    return read_success ? count : 0;
}

void PiPicoComm::write(unsigned char* buf, const size_t count) {
    uart.send_bytes(buf, count);
}

void PiPicoComm::mutex_lock() {
    mutex_enter_blocking(&mutex);
}

void PiPicoComm::mutex_unlock() {
    mutex_exit(&mutex);
}
