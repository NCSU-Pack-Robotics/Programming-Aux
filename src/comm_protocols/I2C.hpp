#pragma once

#include <hardware/i2c.h>

#include <utility>
#include <vector>

/** Simple I2C wrapper and helper for Raspberry Pi Pico */
class I2C {
public:

    /**
     * Creates a new I2C instance.
     * @param device_addr The 7 bit I2C address of the device to communicate with.
     * @param baud_rate The baud rate for communication. Must be between 0 and 100kHz.
     * @param i2c_id The i2c identifier to use for this connection.
     * The pico has 2 I2C lines defined by macros `i2c0_inst` and `i2c1_inst`.
     * @param pins The pair of GPIO pins used for i2c communication.
     * The first is SDA, second is SCL.
     */
    I2C(uint8_t device_addr,
        uint baud_rate,
        i2c_inst_t *i2c_id,
        const std::pair<uint, uint>& pins);

    /**
     * Writes data to a register on the device.
     * @param reg The register to write to.
     * @param data The data to write. Will be written in order, so the first byte will be written to `reg`, the second to `reg + 1`, etc.
     * @return The number of bytes written.
     */
    int write_reg(uint8_t reg, const std::vector<uint8_t>& data) const;

    /**
     * Reads bytes from a register on the device.
     * @param reg The register to read from.
     * @param num_bytes The number of bytes to read.
     * @return The bytes read from the device.
     */
    std::vector<uint8_t> read_reg(uint8_t reg, size_t num_bytes) const;

    ~I2C();

private:
    /** The 7 bit I2C address of the device to communicate with. */
    const uint8_t device_addr;

    /** The i2c identifier to use for this connection.
     * The pico has 2 I2C lines defined by macros `i2c0_inst` and `i2c1_inst`. */
    i2c_inst_t *i2c_id;

    /** The baud rate for communication. */
    const uint baud_rate;

    /** The pair of GPIO pins used for i2c communication.
     * The first is SDA, second is SCL. */
    const std::pair<uint, uint> pins;
};
