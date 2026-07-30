#pragma once

#include <sys/types.h>
#include <pico/sync.h>

#include "I2C.hpp"
#include "Uart.hpp"
#include "comms/AbstractComm.hpp"
#include "config.hpp"

/** Communication abstraction for the Raspberry Pi Pico 2 - rp2350A.
 * The pico communicates over UART */
class PiPicoComm : public AbstractComm {
    mutex_t mutex{};

    /** Uart instance to use for communicating with the brain. */
    const Uart uart{Config::BRAIN_COMM::BAUD_RATE,
                    Config::BRAIN_COMM::UART_ID,
                    {Config::Pins::UART_TX_GPIO, Config::Pins::UART_RX_GPIO}};

public:
    PiPicoComm() = default;
    ~PiPicoComm() override;
    size_t read(unsigned char* buf, size_t count) override;
    void write(unsigned char* buf, size_t count) override;

    void mutex_lock() override;
    void mutex_unlock() override;
};
