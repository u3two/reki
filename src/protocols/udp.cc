#include "udp.hpp"
#include "../visitors/visitors.hpp"

#include <iomanip>
#include <iostream>
#include <arpa/inet.h>

template<>
void UDP_Header::print() const
{
    std::cout << "[UDP Header]\n";

    std::cout << "source port: " << +m_header.source_port << std::endl;
    std::cout << "destination port: " << +m_header.destination_port << std::endl;

    std::cout << "length : " << +m_header.length << std::endl;

    std::cout << std::hex << std::uppercase 
              << "checksum: 0x" << std::setw(4) << std::setfill('0') << +m_header.length << std::endl
              << std::dec;
}

template<>
void UDP_Header::into_host_endian()
{
    m_header.source_port = ntohs(m_header.source_port);
    m_header.destination_port = ntohs(m_header.destination_port);
    m_header.length = ntohs(m_header.length);
    m_header.checksum = ntohs(m_header.checksum);
}

UDP_Packet::UDP_Packet(std::vector<u8>&& bytes) 
: super{std::move(bytes)}
, m_header{offset_ptr()}
{
    m_header.into_host_endian();
    m_offset += sizeof(UDP_Header);
}

UDP_Packet::UDP_Packet(super&& sup)
    : super{std::move(sup)}
    , m_header{offset_ptr()} 
{
    m_header.into_host_endian();
}

void UDP_Packet::apply(visitors::PacketVisitor &visitor) 
{
    visitor.visit(*this);
}
