#include <hardware/gpio.h>
#include <iostream>
#include <thread>
#include <cstdio>

#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "config.hpp"
#include "SerialHandler.hpp"
#include "comms/PiPicoComm.hpp"
#include "devices/PAA5160E1.hpp"
#include "packets/InitializeOpticalPacket.hpp"
#include "packets/OpticalPacket.hpp"

static void hello_world() {
    std::cout << "Program Started" << std::endl;

    gpio_init(Config::Pins::STATUS_LED_GPIO);
    gpio_set_dir(Config::Pins::STATUS_LED_GPIO, GPIO_OUT);
    gpio_put(Config::Pins::STATUS_LED_GPIO, true);
}

static SerialHandler serial_handler{std::make_unique<PiPicoComm>()};
const I2C i2c_instance(Config::OPTICAL_SENSOR::ADDR, Config::OPTICAL_SENSOR::BAUD_RATE,
                           Config::OPTICAL_SENSOR::I2C_ID,
                           {Config::Pins::OPTICAL_SDA_GPIO, Config::Pins::OPTICAL_SCL_GPIO});
static PAA5160E1 odom_sensor(i2c_instance);

static void receive_core() {
    while (true) {
        serial_handler.receive();
        printf("Received packet!\n");
        auto packet = serial_handler.pop_latest<OpticalPacket>();
        if (packet.has_value()) {
            printf("counter at %lf\n", packet->get_data<OpticalPacket>().x);
        }
        // sleep_ms(100);
    }
}

int main() {
    stdio_init_all();
    hello_world();

    // Init optical sensor in separate scope for no reason
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

    sleep_ms(100);

    multicore_launch_core1(receive_core);

    while (true) {
        const auto measurement = odom_sensor.get_position();
        printf("X: %.2f | Y: %.2f | H: %.2f\n", measurement.x, measurement.y, measurement.h * (180.0 / M_PI));

        OpticalPacket packet{measurement.x, measurement.y, measurement.h};
        serial_handler.send(packet);

        sleep_ms(100);
    }
}
