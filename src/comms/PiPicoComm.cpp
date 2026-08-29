#include "PiPicoComm.hpp"

PiPicoComm::PiPicoComm() {
    mutex_init(&mutex);
}

PiPicoComm::~PiPicoComm() {
    mutex_exit(&mutex);
}

size_t PiPicoComm::read(unsigned char* buf, const size_t count) {
    return uart.read_bytes(buf, count);
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
