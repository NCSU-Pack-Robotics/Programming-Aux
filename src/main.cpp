#include <hardware/gpio.h>
#include <iostream>
#include <thread>

#include "config.hpp"
#include "pico/stdlib.h"

#include "comms/Uart.hpp"
#include "devices/PAA5160E1.hpp"
#include "packets/OpticalPacket.hpp"
#include "SerialHandler.hpp"

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

static void hello_world() {
    std::cout << "Program Started" << std::endl;

    gpio_init(Config::Pins::STATUS_LED_GPIO);
    gpio_set_dir(Config::Pins::STATUS_LED_GPIO, GPIO_OUT);
    gpio_put(Config::Pins::STATUS_LED_GPIO, true);
}

int main() {
    // using namespace std::chrono_literals;

    stdio_init_all();

	// std::vector<std::thread> threads;
 //
 //    SerialHandler serial_handler{std::make_unique<PiPicoComm>()};
 //
	// serial_handler.add_listener<InitializeOpticalPacket>([&threads, &serial_handler](const Packet& packet) {
	// 	// Get the file descriptor from the optical sensor bus
	// 	// int fd = wiringPiI2CSetup(DEVICE_ADDR);
	// 	// TODO: What do we do on error?
	// 	// wiringPiI2CWriteReg8(fd, RESET_REG, true); // Reset tracking
	// 	// wiringPiI2CWriteReg8(fd, IMU_CALIBRATION_REG, 255); // Number of samples for calibration. Each one takes 3ms so fewer can speed up total calibration time.
	// 	// do
	// 	// {
	// 		// std::this_thread::sleep_for(3ms);
	// 	// } while (wiringPiI2CReadReg8(fd, IMU_CALIBRATION_REG) != 0);
	// 	serial_handler.send(InitializeOpticalPacket{});
	// 	// threads.emplace_back(send_position_thread, fd, std::ref(serial_handler));
	// });

    hello_world();

    const I2C i2c_instance(Config::OPTICAL_SENSOR::ADDR, Config::OPTICAL_SENSOR::BAUD_RATE,
                           Config::OPTICAL_SENSOR::I2C_ID,
                           {Config::Pins::OPTICAL_SDA_GPIO, Config::Pins::OPTICAL_SCL_GPIO});
    PAA5160E1 odom_sensor(i2c_instance);

    sleep_ms(100);

    bool success = true;
    success &= odom_sensor.calibrate();
    success &= odom_sensor.self_test();
    success &= odom_sensor.reset();

    while (!success) {
        printf("Failed to initialize the odometry sensor\n");
        sleep_ms(100);

        success = true;
        success &= odom_sensor.calibrate();
        success &= odom_sensor.self_test();
        success &= odom_sensor.reset();
    }

    const Uart uart{Config::BRAIN_COMM::BAUD_RATE,
                    Config::BRAIN_COMM::UART_ID,
                    {Config::Pins::UART_TX_GPIO, Config::Pins::UART_RX_GPIO}};

    while (true) {
        // serial_handler.receive();
        const auto measurement = odom_sensor.get_position();
        printf("X: %.2f | Y: %.2f | H: %.2f\n", measurement.x, measurement.y, measurement.h * (180.0 / M_PI));

        constexpr uint8_t data[] = "Sent from Pico2";
        uart.send_bytes(data, sizeof(data));

        uint8_t buff[sizeof(data)] = {};
        uart.read_bytes(buff, sizeof(buff));
        printf("Received: %s\n", buff);

        sleep_ms(1000);
    }

	// for (auto& thread : threads) {
	// 	if (thread.joinable()) {
	// 		thread.join();
	// 	}
	// }
}
