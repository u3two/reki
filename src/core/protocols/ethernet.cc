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

std::string ethertype_to_string(EtherType et)
{
    std::ostringstream oss;

    u16 as_u16 = static_cast<u16>(et);
    if (as_u16 <= 1500) {
        oss << std::dec << "size(" << as_u16 << ")\n";
    } else if (as_u16 <= 1535) {
        oss << "UNDEFINED (malformed packet?)";
    } else {
        oss << "0x" << as_u16 
                  << " (" << ethertype_to_sv(et) << ")"
                  << std::endl;
    }

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

    std::cout << "EtherType: " << ethertype_to_string(EtherType(m_header.ethertype));

    std::cout << std::dec;
}

void EthernetPacket::apply(PacketVisitor &visitor) 
{
    visitor.visit(*this);
}
