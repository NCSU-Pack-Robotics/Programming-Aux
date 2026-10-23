#include "packetFunctions.hpp"
/**
* Functions based on incoming packet id. Modify when needed.
*/
void packetFunctions::IntitializeAux(SerialHandler* serial_handler){

}
void packetFunctions::Optical(SerialHandler* serial_handler) {
    mutex_enter_blocking(&measurementMut);
    serial_handler->send(OpticalPacket{measurement.x, measurement.y, measurement.h});
    mutex_exit(&measurementMut);
}
void packetFunctions::InitializeOptical(SerialHandler* serial_handler){

}
void packetFunctions::InitializeOpticalComplete(SerialHandler* serial_handler){

}
void packetFunctions::TextPacket(SerialHandler* serial_handler){

}
