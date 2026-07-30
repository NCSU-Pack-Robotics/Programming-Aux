#include "Uart.hpp"

#include "pico/stdlib.h"
#include "hardware/uart.h"

#include <cstdio>

Uart::Uart(const uint baud_rate,
        uart_inst_t *uart_id,
        const std::pair<uint8_t, uint8_t> &pins)
    : baud_rate(baud_rate), uart_id(uart_id), pins(pins) {

    gpio_set_function(pins.first, UART_FUNCSEL_NUM(uart_id, pins.first));
    gpio_set_function(pins.second, UART_FUNCSEL_NUM(uart_id, pins.second));
    uart_init(uart_id, baud_rate);

    sleep_ms(10); // Short delay to ensure pins are set up before UART communication
}

void Uart::flush_rx() const {
    while (uart_is_readable(uart_id))
        (void) uart_getc(uart_id);
}

void Uart::send_byte(const uint8_t byte) const {
    send_bytes(&byte, 1);
}

void Uart::send_bytes(const uint8_t *bytes, const size_t length) const {
    // Transmit and wait
    uart_write_blocking(uart_id, bytes, length);
    uart_tx_wait_blocking(uart_id);
}

bool Uart::read_bytes(uint8_t *buffer, const size_t length, const uint32_t timeout_us) const {
    if (uart_is_readable_within_us(uart_id, timeout_us)) {
        uart_read_blocking(uart_id, buffer, length);
        return true;
    }

    return false;
}

Uart::~Uart() {
    uart_deinit(uart_id);
}
