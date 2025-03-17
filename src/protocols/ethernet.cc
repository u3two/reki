#include "ethernet.hpp"

EthernetPacket::EthernetPacket(std::vector<u8> bytes)
: super{bytes} 
{
    m_header = 
        reinterpret_cast<const EthernetHeader *>(this->bytes().data());
}
