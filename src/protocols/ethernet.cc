#include "ethernet.hpp"

#include "../visitors/visitor.hpp"

#include <iostream>
#include <arpa/inet.h>

EthernetPacket::EthernetPacket(std::vector<u8>&& bytes)
: super{std::move(bytes)} 
, m_header(offset_ptr())
{
    m_header.into_host_endianness();
    m_offset += sizeof(EthernetHeader);
}


void EthernetHeader::into_host_endianness() 
{
    this->ethertype = ntohs(this->ethertype);
}

void EthernetHeader::print() const {
    std::cout << "[Ethernet Header]\n";

    std::cout << std::uppercase << std::hex;

    std::cout << "Destination: ";
    for (int i = 0; i < destination_size; i++)
        std::cout << +this->destination[i]
                  << ((i != destination_size - 1) ? ":" : "");
    std::cout << std::endl;

    std::cout << "Source: ";
    for (int i = 0; i < destination_size; i++)
        std::cout << +this->source[i]
                  << ((i != destination_size - 1) ? ":" : "");
    std::cout << std::endl;

    std::cout << "EtherType: ";
    if (this->ethertype <= 1500) {
        std::cout << std::dec << "size(" << this->ethertype << ")\n";
    } else if (this->ethertype <= 1535) {
        std::cout << "UNDEFINED (malformed packet?)";
    } else {
        std::cout << "0x" << this->ethertype
                  << " (" << ethertype_to_sv(EtherType(this->ethertype)) << ")"
                  << std::endl;
    }

    std::cout << std::dec;
}

void EthernetPacket::apply(PacketVisitor &visitor) 
{
    visitor.visit(*this);
}
