#include "ethernet.hpp"

#include "../visitors/visitor.hpp"

#include <iostream>
#include <arpa/inet.h>

void EthernetHeader::into_host_endianness() 
{
    this->ethertype = htons(this->ethertype);
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

    u16 as_host = ntohs(this->ethertype);
    std::cout << "EtherType: ";
    if (as_host <= 1500) {
        std::cout << std::dec << "size(" << as_host << ")\n";
    } else if (as_host <= 1535) {
        std::cout << "UNDEFINED (malformed packet?)";
    } else {
        std::cout << "0x" << as_host;
        switch (as_host) {
            case static_cast<u16>(EtherType::IPv4): 
                std::cout << " (IPv4)";
                break;
            case static_cast<u16>(EtherType::IPv6): 
                std::cout << " (IPv6)";
                break;
            case static_cast<u16>(EtherType::WakeOnLan): // ether-wake
                std::cout << " (Wake-on-Lan)";
                break;
        }
        std::cout << std::endl;
    }

    std::cout << std::dec;
}

EthernetPacket::EthernetPacket(std::vector<u8>&& bytes)
: super{std::move(bytes)} 
{
    auto header_mut = reinterpret_cast<EthernetHeader*>(this->bytes_mut().data() + m_offset);
    header_mut->into_host_endianness();
    m_header = header_mut;
    m_offset += sizeof(EthernetHeader);
}

void EthernetPacket::apply(PacketVisitor &visitor) 
{
    visitor.visit(*this);
}
