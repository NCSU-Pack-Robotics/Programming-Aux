#include <cmath>
#include <cstdint>
#include <iostream>
#include <fcntl.h>
#include <vector>

#include "Programming-Push-Back-Common/COBS.hpp"
#include "Programming-Push-Back-Common/SerialHandler.hpp"


struct test_struct
{
    int32_t a;
    int32_t b;
    int32_t c;
};

int main()
{
    SerialHandler serial_handler(DeviceType::PI);

    serial_handler.structs_to_packet_ids.emplace(std::type_index(typeid(test_struct)), PacketId::Hello);
    serial_handler.handlers.emplace(PacketId::Hello, [](const uint8_t* data) {
        const test_struct* test = reinterpret_cast<const test_struct*>(data);
        printf("Received packet with a: %d, b: %d, c: %d\n", test->a, test->b, test->c);
    });

    std::vector<uint8_t> data;

    while (true)
    {
        serial_handler.receive();
    }
}
