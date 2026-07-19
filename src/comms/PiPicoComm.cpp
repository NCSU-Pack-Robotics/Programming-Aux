#include "PiPicoComm.hpp"

#include <hardware/gpio.h>
#include <hardware/uart.h>

PiPicoComm::PiPicoComm() {
    // Initialize UART
    gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_INSTANCE, UART_TX_PIN));
    gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_INSTANCE, UART_RX_PIN));

    uart_init(UART_INSTANCE, UART_BAUDRATE);
}

PiPicoComm::~PiPicoComm() {
    // Deinitialize UART
    uart_deinit(UART_INSTANCE);

    mutex_exit(&mutex);
}

size_t PiPicoComm::read(unsigned char* buf, size_t count) {
    if (count == 0) return 0;

    // Block until at least 1 byte is available
    uart_read_blocking(UART_INSTANCE, buf, 1);

    size_t amt_read = 1;

    // Read as many as are available, and don't go over count, mirroring the read syscall
    while (amt_read < count && uart_is_readable(UART_INSTANCE)) {
        buf[amt_read++] = uart_getc(UART_INSTANCE);
    }

    return amt_read;
}

void PiPicoComm::write(unsigned char* buf, size_t count) {
    uart_write_blocking(UART_INSTANCE, buf, count);
}

void PiPicoComm::mutex_lock() {
    mutex_enter_blocking(&mutex);
}

void PiPicoComm::mutex_unlock() {
    mutex_exit(&mutex);
}
