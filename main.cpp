#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
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
    // // Open with read/write and O_NOCTTY is so we don't become the process's controlling terminal
    // // TODO: Find the right file based on metadata
    // int fd = open("/dev/ttyACM1", O_RDWR | O_NOCTTY);
    // if (fd < 0) {
    //     perror("Error opening file: ");
    //     return 1;
    // }

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

    return 0;
}







// while (true) {
//
//     char c;
//     read(fd, &c, 1);
//     if (c == '\0')
//     {
//         std::optional<std::vector<uint8_t>> decoded = cobs_decode(data);
//         if (!decoded.has_value()) continue;
//
//         if (decoded->size() == sizeof(test_struct) + 8)
//         {
//             test_struct* test = reinterpret_cast<test_struct*>(decoded->data() + 8);
//             printf("a value: %d\n", test->a);
//         }
//         data.clear();
//     }
//     else
//     {
//         // printf("%0.2x ", c);
//         // fflush(stdout);
//         data.push_back(c);
//     }
// }