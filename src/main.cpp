#include <hardware/gpio.h>
#include <iostream>
#include <thread>
#include <cstdio>
#include <functional>
#include <unordered_map>

#include "pico/mutex.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "config.hpp"
#include "SerialHandler.hpp"
#include "comms/PiPicoComm.hpp"
#include "devices/PAA5160E1.hpp"
#include "packets/InitializeAuxPacket.hpp"
#include "packets/InitializeOpticalCompletePacket.hpp"
#include "packets/InitializeOpticalPacket.hpp"
#include "packets/OpticalPacket.hpp"
#include "packets/TextPacket.hpp"
#include "PacketFunctions.hpp"

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

std::unordered_map<uint8_t, std::function<void(SerialHandler&)>> IDPairs = {
    {PacketIds::INITIALIZE_AUX, PacketFunctions::IntitializeAux}, 
    {PacketIds::OPTICAL, PacketFunctions::Optical}, 
    {PacketIds::INITIALIZE_OPTICAL, PacketFunctions::InitializeOptical}, 
    {PacketIds::INITIALIZE_OPTICAL_COMPLETE, PacketFunctions::InitializeOpticalComplete}, 
    {PacketIds::TEXT, PacketFunctions::TextPacket}};

static void receive_core() {
    while (true) {
        const auto& packet = serial_handler.receive_packet();
        if (packet.has_value() && packet->is_request()) {
            auto packet_function = IDPairs.find(packet->get_id());
            if (packet_function != IDPairs.end()){
                packet_function->second(serial_handler);
            }
        }
    }
}

int main() {
    stdio_init_all();
    hello_world();

    mutex_init(&PacketFunctions::measurement_mut);
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
        mutex_enter_blocking(&PacketFunctions::measurement_mut);
        PacketFunctions::measurement = odom_sensor.get_position();
        mutex_exit(&PacketFunctions::measurement_mut);
        sleep_ms(10);
    }
}
