#include "PAA5160E1.hpp"

#include <cstdio>

PAA5160E1::PAA5160E1(const I2C& i2c) : i2c(i2c) {}

bool PAA5160E1::calibrate(uint8_t numSamples, const bool waitUntilDone) const {
    printf("Calibrating IMU with %d samples\n", numSamples);

    // Write the number of samples to the device
    const int num_written = i2c.write_reg(ImuCalib, {numSamples});
    if (num_written != 1) return false;

    // Wait 1 sample period (2.4ms) to ensure the register updates
    sleep_ms(3);

    if (!waitUntilDone) return true;

    // Wait for the calibration to finish, which is indicated by the IMU
    // calibration register reading zero, or until we reach the maximum number
    // of read attempts
    for (uint8_t numAttempts = numSamples; numAttempts > 0; --numAttempts) {
        // Read the gryo calibration register value
        const uint8_t calibrationValue = i2c.read_reg(ImuCalib)[0];

        printf("Calibration value %d\n", calibrationValue);

        // Check if calibration is done
        if (calibrationValue <= 0) return true;

        // Give a short delay between reads. As of firmware v1.0, samples take
        // 2.4ms each, so 3ms should guarantee the next sample is done. This
        // also ensures the max attempts is not exceeded in normal operation
        sleep_ms(3);
    }

    return false;
}
bool PAA5160E1::self_test() const {
    bool success = true;

    // Being test by writing a 1 to bit 0
    success &= i2c.write_reg(SelfTest, {0x01});

    // Loop until self-test is done, should only take ~20ms as of firmware v1.0
    for (int i = 0; i < 10; i++) {
        sleep_ms(5);

        const bool is_testing = i2c.read_reg(SelfTest)[0] & 0x02;  // Test is running while bit 1 is 1.
        if (!is_testing) break;
    }

    // Return success or fail
    return i2c.read_reg(SelfTest)[0] & 0x04;  // Test passed if bit 2 is 1.
}

bool PAA5160E1::reset() const {
    // Write the reset command to the device
    const int num_written = i2c.write_reg(Reset, {0x01});
    if (num_written != 1) return false;

    return true;

}
bool PAA5160E1::set_linear_scalar(float scalar) const {
    if (scalar < 0.872 || scalar > 1.127) return false;

    // Convert to integer, multiples of 0.1% (+0.5 to round instead of truncate)
    uint8_t rawScalar = (int8_t)((scalar - 1.0f) * 1000 + 0.5f);
    return i2c.write_reg(ScalarLinear, {rawScalar}) == 1;
}

bool PAA5160E1::set_angular_vector(float scalar) const {
    if (scalar < 0.872 || scalar > 1.127) return false;

    // Convert to integer, multiples of 0.1% (+0.5 to round instead of truncate)
    uint8_t rawScalar = (int8_t)((scalar - 1.0f) * 1000 + 0.5f);
    return i2c.write_reg(ScalarAngular, {rawScalar}) == 1;
}

PAA5160E1::SensorData PAA5160E1::get_position() const {
    return decode_regs(i2c.read_reg(PosXL, 6));
}

PAA5160E1::SensorData PAA5160E1::get_velocity() const {
    return decode_regs(i2c.read_reg(VelXL, 6));
}

PAA5160E1::SensorData PAA5160E1::get_acceleration() const {
    return decode_regs(i2c.read_reg(AccXL, 6));
}

PAA5160E1::SensorData PAA5160E1::get_position_std_devs() const {
    return decode_regs(i2c.read_reg(PosStdXL, 6));
}

PAA5160E1::SensorData PAA5160E1::get_velocity_std_devs() const {
    return decode_regs(i2c.read_reg(VelStdXL, 6));
}
PAA5160E1::SensorData PAA5160E1::get_acceleration_std_devs() const {
    return decode_regs(i2c.read_reg(AccStdXL, 6));
}

PAA5160E1::SensorData PAA5160E1::decode_regs(const std::vector<uint8_t>& raw_data) {
    if (raw_data.size() != 6) {
        printf("Given %d bytes to decode instad of 6\n", raw_data.size());
        return {};
    }

    const int16_t rawX = static_cast<int16_t>((raw_data[1] << 8) | raw_data[0]);
    const int16_t rawY = static_cast<int16_t>((raw_data[3] << 8) | raw_data[2]);
    const int16_t rawZ = static_cast<int16_t>((raw_data[5] << 8) | raw_data[4]);

    SensorData data;
    data.x = static_cast<float>(rawX) * kInt16ToMeter * kMeterToInch;
    data.y = static_cast<float>(rawY) * kInt16ToMeter * kMeterToInch;
    data.h = static_cast<float>(rawZ) * kInt16ToRad;

    return data;
}
