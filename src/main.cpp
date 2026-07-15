#include <vector>

#include "SerialHandler.hpp"
// #include <wiringPi.h>
// #include <wiringPiI2C.h>
#include <thread>
#include <iostream>

#include "packets/InitializeOpticalPacket.hpp"
#include "packets/OpticalPacket.hpp"

/** Optical Sensor address on I2C bus */
constexpr uint8_t DEVICE_ADDR = 0x17;

constexpr uint8_t IMU_CALIBRATION_REG = 0x06;
constexpr uint8_t RESET_REG = 0x07;
constexpr uint8_t POSITION_REG = 0x20;

/** function that gets ran on another thread for the optical sensor. It's only job is to send position data.
 * @param fd The file descriptor of the optical sensor I2C bus
 * @param serial_handler The serial handler
 */
void send_position_thread(int fd, SerialHandler& serial_handler) {
	uint8_t rawData[6];
	while (true)
    {
		// Read data from the position register
        // wiringPiI2CReadBlockData(fd, POSITION_REG, rawData, sizeof(rawData));

		// taking numbers byte by byte and put into bigger value
        int16_t rawX = (rawData[1] << 8) | rawData[0];
        int16_t rawY = (rawData[3] << 8) | rawData[2];
       	int16_t rawH = (rawData[5] << 8) | rawData[4];

        constexpr double METER_TO_INT16 = 32768.0 / 10.0;
        constexpr double INT16_TO_METER = 1.0 / METER_TO_INT16;

       	constexpr double RAD_TO_INT16 = 32768 / 1.0;
        constexpr double INT16_TO_RAD = 1.0 / RAD_TO_INT16;

        printf("X: %.2f | Y: %.2f | H: %.2f\n",
            rawX * INT16_TO_METER,
            rawY * INT16_TO_METER,
            rawH * INT16_TO_RAD);

		// Send data to the brain
		serial_handler.send(OpticalPacket{rawX * INT16_TO_METER * 1000, rawY * INT16_TO_METER*1000, rawH * INT16_TO_RAD});

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}

int main() {

    using namespace std::chrono_literals;

	std::vector<std::thread> threads;

    SerialHandler serial_handler{};

	serial_handler.add_listener<InitializeOpticalPacket>([&threads](SerialHandler& serial_handler, const Packet& packet) {
		// Get the file descriptor from the optical sensor bus
		// int fd = wiringPiI2CSetup(DEVICE_ADDR);
		// TODO: What do we do on error?
		// wiringPiI2CWriteReg8(fd, RESET_REG, true); // Reset tracking
		// wiringPiI2CWriteReg8(fd, IMU_CALIBRATION_REG, 255); // Number of samples for calibration. Each one takes 3ms so fewer can speed up total calibration time.
		// do
		// {
			// std::this_thread::sleep_for(3ms);
		// } while (wiringPiI2CReadReg8(fd, IMU_CALIBRATION_REG) != 0);
		serial_handler.send(InitializeOpticalPacket{});
		// threads.emplace_back(send_position_thread, fd, std::ref(serial_handler));
	});




    std::vector<uint8_t> data;

    while (true) {
        serial_handler.receive();
    }

	for (auto& thread : threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}
}
