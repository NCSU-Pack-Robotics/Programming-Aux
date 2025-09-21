#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

#include "Programming-Push-Back-Common/COBS.hpp"


struct test_struct
{
    int x;
    int y;
    int z;
    float w;
    double q;
};

int main()
{
    // Open with read/write and O_NOCTTY is so we don't become the process's controlling terminal
    // TODO: Find the right file based on metadata
    int fd = open("/dev/ttyACM1", O_RDWR | O_NOCTTY);
    if (fd < 0) {
        perror("Error opening file: ");
        return 1;
    }

    // test_struct test{};
    // test.x = 1;
    // test.y = 2;
    // test.z = 5;
    // test.w = 1.75F;
    // test.q = 3.14159265;

    std::vector<uint8_t> data;

    while (true) {

        char c;
        read(fd, &c, 1);
        if (c == '\0')
        {
            std::vector<uint8_t> decoded = cobs_decode(data);
            if (decoded.size() == sizeof(test_struct))
            {
                test_struct* test = reinterpret_cast<test_struct*>(decoded.data());
                printf("W value: %f\n", test->w);
            }
            data.clear();
        }
        else
        {
            printf("%0.2x ", c);
            data.push_back(c);
        }
    }

    close(fd);
    return 0;
}
