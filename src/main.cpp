#include <hardware/gpio.h>
#include <iostream>
#include <thread>
#include <cstdio>
#include <functional>
#include <unordered_map>

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

PAA5160E1::SensorData measurement{};

/**
 * Functions based on incoming packet id.
 */
void IntitializeAuxFunction(){

}

void OpticalFunction() {
    serial_handler.send(OpticalPacket{measurement.x, measurement.y, measurement.h});
}

void InitializeOpticalFunction(){

}

void InitializeOpticalCompleteFunction(){

}

void TextPacketFunction(){

}

std::unordered_map<uint8_t, std::function<void()>> IDPairs = {
    {PacketIds::INITIALIZE_AUX, IntitializeAuxFunction}, 
    {PacketIds::OPTICAL, OpticalFunction}, 
    {PacketIds::INITIALIZE_OPTICAL, InitializeOpticalFunction}, 
    {PacketIds::INITIALIZE_OPTICAL_COMPLETE, InitializeOpticalCompleteFunction}, 
    {PacketIds::TEXT, TextPacketFunction}};

static void receive_core() {
    while (true) {
        const auto& packet = serial_handler.receive_packet();
        if (packet.has_value() && packet->is_request()) {
            auto packet_function = IDPairs.find(packet->get_id());
            if (packet_function != IDPairs.end()){
                packet_function->second();
            }
        }
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

        measurement = odom_sensor.get_position();
        sleep_ms(10);
    }
}
