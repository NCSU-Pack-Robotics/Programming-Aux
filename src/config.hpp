#pragma once

#include <hardware/i2c.h>
#include <hardware/uart.h>
#include <pico/types.h>

namespace Config {
    /** Any physical pin or GPIO numbers that may be referenced throughout the program. */
    namespace Pins {
        /** GPIO used for the serial data of the i2c line communicating with the optical sensor. */
        constexpr uint OPTICAL_SDA_GPIO = 0;

        /** GPIO usd for the serial clock of the i2c line communication with the optical sensor. */
        constexpr uint OPTICAL_SCL_GPIO = 1;

        /** GPIO used for transmitting data from the pico to the brain. */
        constexpr uint UART_TX_GPIO = 4;

        /** GPIO used for receiving data sent from the brain to the pico. */
        constexpr uint UART_RX_GPIO = 5;

        /** Used to display the status of the program. At the very least, this LED turns on when the program is
         * running. */
        constexpr uint STATUS_LED_GPIO = 25;

        /** This pin is pulled high enabling the driver by default.
         * If this pin is actively pulled low, the driver will turn off. */
        constexpr uint DRIVER_ENABLE_GPIO = 28;
    }

    /** Parameters used for communicating with the brain over UART/RS-485 smart port connection. */
    namespace BRAIN_COMM {
        /** Number of bits/second to send and receive from the brain. */
        constexpr unsigned int BAUD_RATE = 115200;

        /** UART instance 0 or 1 . */
        inline auto UART_ID = uart1;
    }

    /** Parameters used for communicating with the SparkFun odometry sensor. */
    namespace OPTICAL_SENSOR {
        /** Load cell amplifier's I2C address. */
        constexpr uint8_t ADDR = 0x17;

        /** I2C instance 0 or 1. */
        inline auto I2C_ID = &i2c0_inst;

        /** Number of bits/second to send and receive from the optical sensor.. */
        constexpr unsigned int BAUD_RATE = 9600;
    }
}
