#pragma once

#include "Uart.hpp"
#include "hardware/uart.h"

#include <utility>

/** Simple UART wrapper and helper for Raspberry Pi Pico */
class Uart {
public:
    /**
     * Creates a new Pico_Uart instance.
     * @param baud_rate The baud rate for communication.
     * @param uart_id The uart identifier for this connection.
     * The pico2 has 2 uart buses defined by macros `uart0` and `uart1`.
     * @param pins The GPIO numbers used for UART communication. The first is Tx, second is Rx.
     */
    Uart(unsigned int baud_rate,
            uart_inst_t *uart_id,
            std::pair<uint8_t, uint8_t> pins);

    /**
     * Sends a single byte over the UART connection.
     * This will nuke everything in the receive buffer.
     * @param byte The byte to send.
     */
    void send_byte(uint8_t byte) const;

    /**
    * Sends an array of bytes over the UART connection.
    * This will nuke everything in the receive buffer.
    * @param bytes Pointer to the first byte to send.
    * @param length The number of bytes to send.
    */
    void send_bytes(const uint8_t *bytes, size_t length) const;

    /**
     * Reads bytes from the UART connection into a buffer. This is a blocking call.
     * @param buffer Pointer to the buffer to read bytes into. Must be at least `length` bytes long.
     * @param length The maximum amount of bytes to read.
     * @return The number of bytes read.
     */
    size_t read_bytes(uint8_t *buffer, size_t length) const;

    ~Uart();

private:
    /** Discards all bytes currently waiting in the RX FIFO. */
    void flush_rx() const;

    /** The baud rate for communication. */
    const uint baud_rate;

    /** The uart identifier for this connection.
     * The pico2 has 2 uart buses defined by macros `uart0` and `uart1`. */
    uart_inst_t *uart_id;

    /** The pair of GPIO pins used for UART communication.
     * The first is Tx, second is Rx. */
    const std::pair<uint8_t, uint8_t> pins;
};
