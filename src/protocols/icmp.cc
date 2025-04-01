#include "icmp.hpp"
#include "../visitors/visitors.hpp"

#include <iostream>
#include <arpa/inet.h>

template<>
void ICMP_Header::into_host_endian()
{
    m_header.checksum = ntohs(m_header.checksum);
}

template<>
void ICMP_Header::print() const
{
    std::cout << "[ICMP Header]\n";

    std::cout << "Type: " << m_header.type << std::endl
              << "Code: " << m_header.code << std::endl;
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


void ICMP_Packet::apply(PacketVisitor &visitor) 
{
    visitor.visit(*this);
}
