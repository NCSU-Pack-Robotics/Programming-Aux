#include <vector>

#include "SerialHandler.hpp"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <thread>
#include <iostream>

#include "packets/InitializeOpticalPacket.hpp"
#include "packets/OpticalPacket.hpp"

constexpr uint8_t DEVICE_ADDR = 0x17;

constexpr uint8_t IMU_CALIBRATION_REG = 0x06;
constexpr uint8_t RESET_REG = 0x07;
constexpr uint8_t POSITION_REG = 0x20;

void send_position_thread(int fd, SerialHandler& serial_handler)
{
	uint8_t rawData[6];
	while (true)
    	{
	        // wiringPiI2CReadBlockData(fd, POSITION_REG, rawData, 6);

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

		serial_handler.send(OpticalPacket{OpticalPacket::Data{rawX * INT16_TO_METER * 1000, rawY * INT16_TO_METER*1000, rawH * INT16_TO_RAD}});

	    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

int main() {

    using namespace std::chrono_literals;

    SerialHandler serial_handler{};

	serial_handler.add_listener<InitializeOpticalPacket>([](SerialHandler& serial_handler, const Packet&) {
		std::cout << "listener" << std::endl;
		int fd = wiringPiI2CSetup(DEVICE_ADDR);
		// TODO: What do we do on error?
		wiringPiI2CWriteReg8(fd, RESET_REG, true); // Reset tracking
		wiringPiI2CWriteReg8(fd, IMU_CALIBRATION_REG, 255); // Number of samples for calibration. Each one takes 3ms so fewer can speed up total calibration time.
		do
		{
		    std::this_thread::sleep_for(3ms);
		} while (wiringPiI2CReadReg8(fd, IMU_CALIBRATION_REG) != 0);
		std::cout << "sent" << std::endl;
		serial_handler.send(InitializeOpticalPacket{});
		std::thread position_thread{send_position_thread, fd, std::ref(serial_handler)};
		position_thread.detach();
	});




    std::vector<uint8_t> data;

    while (true) {

        serial_handler.receive();
		std::cout << "received" << std::endl;
    }
}
