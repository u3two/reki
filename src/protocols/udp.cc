#include "udp.hpp"
#include "../visitors/visitors.hpp"

#include <iomanip>
#include <iostream>
#include <arpa/inet.h>

template<>
void UDP_Header::print() const
{
    std::cout << "[UDP Header]\n";

    std::cout << "source port: " << +m_data.source_port << std::endl;
    std::cout << "destination port: " << +m_data.destination_port << std::endl;

    std::cout << "length : " << +m_data.length << std::endl;

    std::cout << std::hex << std::uppercase 
              << "checksum: 0x" << std::setw(4) << std::setfill('0') << +m_data.length << std::endl
              << std::dec;
}

template<>
void UDP_Header::into_host_endian()
{
    m_data.source_port = ntohs(m_data.source_port);
    m_data.destination_port = ntohs(m_data.destination_port);
    m_data.length = ntohs(m_data.length);
    m_data.checksum = ntohs(m_data.checksum);
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
