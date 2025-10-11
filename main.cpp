#include <vector>

#include "common/SerialHandler.hpp"
#include "common/packet/types/encoder.hpp"

struct test_struct {
    int32_t a;
    int32_t b;
    int32_t c;
};

int main() {

    SerialHandler serial_handler{};

    EncoderData encoder_data{3.14159265};
    Packet packet(PacketId::ENCODER, reinterpret_cast<uint8_t*>(&encoder_data), sizeof(EncoderData));
    // serial_handler.send(packet);

    std::vector<uint8_t> data;

    while (true) {
        serial_handler.receive();
        if (std::optional<Packet> packet = serial_handler.pop_latest(PacketId::ENCODER))
        {

        }
    }
}
