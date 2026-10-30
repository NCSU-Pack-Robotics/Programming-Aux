#pragma once
#include "pico/mutex.h"
#include "SerialHandler.hpp"
#include "devices/PAA5160E1.hpp"
#include "packets/InitializeAuxPacket.hpp"
#include "packets/InitializeOpticalCompletePacket.hpp"
#include "packets/InitializeOpticalPacket.hpp"
#include "packets/OpticalPacket.hpp"
#include "packets/TextPacket.hpp"
namespace PacketFunctions{
    inline PAA5160E1::SensorData measurement{};
    inline mutex_t measurement_mut;
    void IntitializeAux(SerialHandler& serial_handler);
    void Optical(SerialHandler& serial_handler);
    void InitializeOptical(SerialHandler& serial_handler);
    void InitializeOpticalComplete(SerialHandler& serial_handler);
    void TextPacket(SerialHandler& serial_handler);
};
