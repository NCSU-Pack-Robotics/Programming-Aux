#pragma once

#include <cmath>

#include "../comms/I2C.hpp"

class PAA5160E1 {
    I2C i2c;
public:
    /** A data chunk read from the sensor.
     * Position, velocity acceleration, standard deviation, and offsets all use this type. */
    using SensorData = union {
        struct {
            float x, y, h;
        };

        float data[6];
    };

    PAA5160E1(const I2C& i2c);

    /**
     * Calibrates the IMU which removes the accelerometer and gyroscope offsets
     * @param numSamples Number of samples to take for calibration. Each sample
     * takes about 2.4ms, so fewer samples can be taken for faster calibration.
     * @param waitUntilDone false to return immediately.
     * @return true for success, false for failure.
     */
    bool calibrate(uint8_t numSamples = 255, bool waitUntilDone = true) const;

    /**
     * Resets positions to origin and resets internal kalman filters.
     * @return true for success, false for failure.
     */
    bool reset() const;

    /**
     * Gets the position from the sensor.
     * @return The position data. The data will be 0s (false) if an error occurred.
     */
    [[nodiscard]] SensorData get_position() const;

protected:
    static constexpr uint8_t ProductId = 0x00;
    static constexpr uint8_t HwVersion = 0x01;
    static constexpr uint8_t FwVersion = 0x02;
    static constexpr uint8_t ScalarLinear = 0x04;
    static constexpr uint8_t ScalarAngular = 0x05;
    static constexpr uint8_t ImuCalib = 0x06;
    static constexpr uint8_t Reset = 0x07;
    static constexpr uint8_t SignalProcess = 0x0E;
    static constexpr uint8_t SelfTest = 0x0F;
    static constexpr uint8_t OffXL = 0x10;
    static constexpr uint8_t OffXH = 0x11;
    static constexpr uint8_t OffYL = 0x12;
    static constexpr uint8_t OffYH = 0x13;
    static constexpr uint8_t OffHL = 0x14;
    static constexpr uint8_t OffHH = 0x15;
    static constexpr uint8_t Status = 0x1F;
    static constexpr uint8_t PosXL = 0x20;
    static constexpr uint8_t PosXH = 0x21;
    static constexpr uint8_t PosYL = 0x22;
    static constexpr uint8_t PosYH = 0x23;
    static constexpr uint8_t PosHL = 0x24;
    static constexpr uint8_t PosHH = 0x25;
    static constexpr uint8_t VelXL = 0x26;
    static constexpr uint8_t VelXH = 0x27;
    static constexpr uint8_t VelYL = 0x28;
    static constexpr uint8_t VelYH = 0x29;
    static constexpr uint8_t VelHL = 0x2A;
    static constexpr uint8_t VelHH = 0x2B;
    static constexpr uint8_t AccXL = 0x2C;
    static constexpr uint8_t AccXH = 0x2D;
    static constexpr uint8_t AccYL = 0x2E;
    static constexpr uint8_t AccYH = 0x2F;
    static constexpr uint8_t AccHL = 0x30;
    static constexpr uint8_t AccHH = 0x31;
    static constexpr uint8_t PosStdXL = 0x32;
    static constexpr uint8_t PosStdXH = 0x33;
    static constexpr uint8_t PosStdYL = 0x34;
    static constexpr uint8_t PosStdYH = 0x35;
    static constexpr uint8_t PosStdHL = 0x36;
    static constexpr uint8_t PosStdHH = 0x37;
    static constexpr uint8_t VelStdXL = 0x38;
    static constexpr uint8_t VelStdXH = 0x39;
    static constexpr uint8_t VelStdYL = 0x3A;
    static constexpr uint8_t VelStdYH = 0x3B;
    static constexpr uint8_t VelStdHL = 0x3C;
    static constexpr uint8_t VelStdHH = 0x3D;
    static constexpr uint8_t AccStdXL = 0x3E;
    static constexpr uint8_t AccStdXH = 0x3F;
    static constexpr uint8_t AccStdYL = 0x40;
    static constexpr uint8_t AccStdYH = 0x41;
    static constexpr uint8_t AccStdHL = 0x42;
    static constexpr uint8_t AccStdHH = 0x43;

    /** Product ID register value */
    static constexpr uint8_t ProductIdVal = 0x5F;

private:
    /**
     * Helper function to turn I2C outputs into usable @c SensorData objects for several types of data read from the
     * sensor.
     * @param raw_data 6 byte data read from the sensor.
     * @return A Sensor data object from the parsed @c raw_data.
     */
    [[nodiscard]] static SensorData decode_regs(const std::vector<uint8_t>& raw_data) ;

    /** Conversion factors */
    static constexpr float kMeterToInch = 39.37f;
    static constexpr float kInchToMeter = 1.0f / kMeterToInch;
    static constexpr float kRadianToDegree = 180.0f / M_PI;
    static constexpr float kDegreeToRadian = M_PI / 180.0f;

    /**
     * Conversion factor for the linear position registers. 16-bit signed
     * registers with a max value of 10 meters (394 inches) gives a resolution
     * of about 0.0003 mps (0.012 ips)
     */
    static constexpr float kMeterToInt16 = 32768.0f / 10.0f;
    static constexpr float kInt16ToMeter = 1.0f / kMeterToInt16;

    /**
     * Conversion factor for the linear velocity registers. 16-bit signed
     * registers with a max value of 5 mps (197 ips) gives a resolution of about
     * 0.00015 mps (0.006 ips)
     */
    static constexpr float kMpsToInt16 = 32768.0f / 5.0f;
    static constexpr float kInt16ToMps = 1.0f / kMpsToInt16;

    /**
     * Conversion factor for the linear acceleration registers. 16-bit signed
     * registers with a max value of 157 mps^2 (16 g) gives a resolution of
     * about 0.0048 mps^2 (0.49 mg)
     */
    static constexpr float kMpssToInt16 = 32768.0f / (16.0f * 9.80665f);
    static constexpr float kInt16ToMpss = 1.0f / kMpssToInt16;

    /**
     * Conversion factor for the angular position registers. 16-bit signed
     * registers with a max value of pi radians (180 degrees) gives a resolution
     * of about 0.00096 radians (0.0055 degrees)
     */
    static constexpr float kRadToInt16 = 32768.0f / M_PI;
    static constexpr float kInt16ToRad = 1.0f / kRadToInt16;

    /**
     * Conversion factor for the angular velocity registers. 16-bit signed
     * registers with a max value of 34.9 rps (2000 dps) gives a resolution of
     * about 0.0011 rps (0.061 degrees per second)
     */
    static constexpr float kRpsToInt16 = 32768.0f / (2000.0f * kDegreeToRadian);
    static constexpr float kInt16ToRps = 1.0f / kRpsToInt16;

    /**
     * Conversion factor for the angular acceleration registers. 16-bit signed
     * registers with a max value of 3141 rps^2 (180000 dps^2) gives a
     * resolution of about 0.096 rps^2 (5.5 dps^2)
     */
    static constexpr float kRpssToInt16 = 32768.0f / (M_PI * 1000.0f);
    static constexpr float kInt16ToRpss = 1.0f / kRpssToInt16;
};
