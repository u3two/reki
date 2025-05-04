#include "icmp.hpp"
#include "../visitors/visitors.hpp"

#include <iostream>
#include <arpa/inet.h>

template<>
void ICMP_Header::into_host_endian()
{
    m_data.checksum = ntohs(m_data.checksum);
}

template<>
void ICMP_Header::print() const
{
    std::cout << "[ICMP Header]\n";

    std::cout << "Type: " << m_data.type << std::endl
              << "Code: " << m_data.code << std::endl;
}

ICMP_Packet::ICMP_Packet(std::vector<u8>&& bytes) 
: super{std::move(bytes)}
, m_header{offset_ptr()}
{
    m_header.into_host_endian();
    // TODO: incomplete, does not fully account for payload etc.
    m_offset += sizeof(ICMP_Header);
}

ICMP_Packet::ICMP_Packet(super&& sup)
    : super{std::move(sup)}
    , m_header{offset_ptr()} 
{
    m_header.into_host_endian();
}


void ICMP_Packet::apply(visitors::PacketVisitor &visitor) 
{
    visitor.visit(*this);
}
