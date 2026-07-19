#pragma once

#include <hardware/uart.h>
#include <sys/types.h>

#include <pico/sync.h>


#include "comms/AbstractComm.hpp"

/** Communication abstraction for the Raspberry Pi Pico 2 - rp2350A.
 * The pico communicates over UART */
class PiPicoComm : public AbstractComm {
    static constexpr uint UART_TX_PIN = 4;
    static constexpr uint UART_RX_PIN = 5;
    static constexpr uint UART_BAUDRATE = 115200;
    static inline uart_inst_t* const UART_INSTANCE = uart1;

    mutex_t mutex{};
public:
    PiPicoComm();
    ~PiPicoComm() override;
    size_t read(unsigned char* buf, size_t count) override;
    void write(unsigned char* buf, size_t count) override;

    void mutex_lock() override;
    void mutex_unlock() override;
};
