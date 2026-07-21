#include "PAA5160E1.hpp"

#include <cstdio>

PAA5160E1::PAA5160E1(const I2C& i2c) : i2c(i2c) {}

bool PAA5160E1::calibrate(uint8_t numSamples, bool waitUntilDone) const {
    // Write the number of samples to the device
    const int num_written = i2c.write_reg(ImuCalib, {numSamples});
    if (num_written != 1) return false;

    // Wait 1 sample period (2.4ms) to ensure the register updates
    sleep_ms(3);

    if (!waitUntilDone) return true;

    // Wait for the calibration to finish, which is indicated by the IMU
    // calibration register reading zero, or until we reach the maximum number
    // of read attempts
    for (uint8_t numAttempts = numSamples; numAttempts > 0; numAttempts--) {
        // Read the gryo calibration register value
        const auto calibrationValue = i2c.read_reg(ImuCalib)[0];

        // Check if calibration is done
        if (calibrationValue <= 0) return true;

        // Give a short delay between reads. As of firmware v1.0, samples take
        // 2.4ms each, so 3ms should guarantee the next sample is done. This
        // also ensures the max attempts is not exceeded in normal operation
        sleep_ms(3);
    }

    return false;
}

bool PAA5160E1::reset() const {
    // Write the reset command to the device
    const int num_written = i2c.write_reg(Reset, {0x01});
    if (num_written != 1) return false;

    return true;

}

PAA5160E1::SensorData PAA5160E1::get_position() const {
    return decode_regs(i2c.read_reg(PosXL, 6));
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
