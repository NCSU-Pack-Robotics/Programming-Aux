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

    EncoderData testData{67.69};
    serial_handler.send(Packet(PacketId::ENCODER, reinterpret_cast<uint8_t*>(&testData), sizeof(testData)));

    std::vector<uint8_t> data;

    while (true) {
        serial_handler.receive();
        if (std::optional<Packet> packet = serial_handler.pop_latest(PacketId::ENCODER))
        {
            EncoderData data = packet->get_data<EncoderData>();
            printf("Data: %f\n", data.value);
        }
    }
}
