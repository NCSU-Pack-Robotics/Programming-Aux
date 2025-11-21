#include <vector>

#include "common/SerialHandler.hpp"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <thread>

#include "common/packet/types/InitializeOpticalComplete.hpp"

static constexpr uint8_t DEVICE_ADDR = 0x17;

static constexpr uint8_t IMU_CALIBRATION_REG = 0x06;
static constexpr uint8_t RESET_REG = 0x07;
static constexpr uint8_t POSITION_REG = 0x20;


int main() {

    using namespace std::chrono_literals;

    SerialHandler serial_handler{};

    serial_handler.add_listener(PacketId::INITIALIZE_OPTICAL, [](SerialHandler& serial_handler, const Packet&) {
        int fd = wiringPiI2CSetup(DEVICE_ADDR);
        // TODO: What do we do on error?
        wiringPiI2CWriteReg8(fd, RESET_REG, true); // Reset tracking
        wiringPiI2CWriteReg8(fd, IMU_CALIBRATION_REG, 255); // Number of samples for calibration. Each one takes 3ms so fewer can speed up total calibration time.
        do
        {
            std::this_thread::sleep_for(3ms);
        } while (wiringPiI2CReadReg8(fd, IMU_CALIBRATION_REG) != 0);

        serial_handler.send(Packet{{PacketId::INITIALIZE_OPTICAL_COMPLETE}, InitializeOpticalComplete{}});
    });


    std::vector<uint8_t> data;

    while (true) {
        serial_handler.receive();
    }
}
