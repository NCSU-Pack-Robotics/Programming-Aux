#include "PacketFunctions.hpp"
/**
* Functions based on incoming packet id. Modify when needed.
*/
void PacketFunctions::IntitializeAux(SerialHandler& serial_handler){

}
void PacketFunctions::Optical(SerialHandler& serial_handler) {
    mutex_enter_blocking(&measurement_mut);
    serial_handler.send(OpticalPacket{measurement.x, measurement.y, measurement.h});
    mutex_exit(&measurement_mut);
}
void PacketFunctions::InitializeOptical(SerialHandler& serial_handler){

}
void PacketFunctions::InitializeOpticalComplete(SerialHandler& serial_handler){

}
void PacketFunctions::TextPacket(SerialHandler& serial_handler){

}
