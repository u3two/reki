#include "ethernet.hpp"

#include "../visitors/visitors.hpp"

#include <ios>
#include <iostream>
#include <arpa/inet.h>

#include <sstream>

std::string mac_to_string(const u8 addr[6])
{
    std::ostringstream oss;
    oss << std::uppercase << std::hex;
    for (u8 i = 0; i < 6; i++)
        oss << +addr[i]
            << ((i != 5) ? ":" : "");
    oss << std::dec;
    return oss.str();
}

EthernetPacket::EthernetPacket(std::vector<u8>&& bytes)
: super{std::move(bytes)} 
, m_header(offset_ptr())
{
    m_header.into_host_endian();
    m_offset += sizeof(EthernetHeader);
}

EthernetPacket::EthernetPacket(super&& sup)
    : super{std::move(sup)}
    , m_header{offset_ptr()} 
{
    m_header.into_host_endian();
    m_offset += sizeof(EthernetHeader);
}

template<>
void EthernetHeader::into_host_endian() 
{
    m_header.ethertype = ntohs(m_header.ethertype);
}

template<>
void EthernetHeader::print() const {
    std::cout << "[Ethernet Header]\n";

    std::cout << std::uppercase << std::hex;

    std::cout << "Destination: " << mac_to_string(m_header.destination) << std::endl;
    std::cout << "Source: " << mac_to_string(m_header.source) << std::endl;

    std::cout << "EtherType: ";
    if (m_header.ethertype <= 1500) {
        std::cout << std::dec << "size(" << m_header.ethertype << ")\n";
    } else if (m_header.ethertype <= 1535) {
        std::cout << "UNDEFINED (malformed packet?)";
    } else {
        std::cout << "0x" << m_header.ethertype
                  << " (" << ethertype_to_sv(EtherType(m_header.ethertype)) << ")"
                  << std::endl;
    }

    std::cout << std::dec;
}

void EthernetPacket::apply(PacketVisitor &visitor) 
{
    visitor.visit(*this);
}
