#include "I2C.hpp"

#include <hardware/gpio.h>
#include <ostream>
#include <pico/binary_info/code.h>


I2C::I2C(const uint8_t device_addr,
         const uint baud_rate,
         i2c_inst_t* i2c_id,
         const std::pair<uint, uint>& pins)
    : device_addr(device_addr), i2c_id(i2c_id), baud_rate(baud_rate), pins(pins) {

    i2c_init(i2c_id, baud_rate);

    gpio_set_function(pins.first, GPIO_FUNC_I2C);
    gpio_set_function(pins.second, GPIO_FUNC_I2C);

    gpio_pull_up(pins.first);
    gpio_pull_up(pins.second);

    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(pins.first, pins.second, GPIO_FUNC_I2C));

    sleep_ms(100); // Short delay to ensure pins are set up before UART communication
}

int I2C::write_reg(const uint8_t reg, const std::vector<uint8_t>& data) const {
    std::vector buff(data);
    buff.insert(buff.begin(), reg);

    const int num_written = i2c_write_timeout_us(i2c_id, device_addr, buff.data(), buff.size(),
        false, 100'000);

    if (num_written != buff.size()) {
        printf("Failed to write to I2C device %02X's register %02X\n",
            static_cast<unsigned int>(device_addr),
            static_cast<unsigned int>(reg));
    }

    return num_written - 1;  // Subtracting to account for the address byte
}

std::vector<uint8_t> I2C::read_reg(const uint8_t reg, const size_t num_bytes) const {
    std::vector<uint8_t> buff(num_bytes);

    i2c_write_timeout_us(i2c_id, device_addr, &reg, 1, true, 100'000);
    i2c_read_timeout_us(i2c_id, device_addr, buff.data(), num_bytes, false, 100'000);

    return buff;
}

I2C::~I2C() {
    i2c_deinit(i2c_id);
}
