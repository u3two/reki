#include "ip.hpp"
#include "../visitors/visitors.hpp"

#include <bitset>
#include <ios>
#include <iostream>
#include <sstream>

#include <arpa/inet.h>

std::string ip_address_to_string(const u8 addr[4])
{
    std::stringstream oss;
    oss << std::dec;
    for (u8 i = 0; i < 4; i++)
        oss << +addr[i]
            << ((i != 3) ? "." : "");
    return oss.str();
}

template<>
void IP_Header::into_host_endian()
{
    m_data.total_length = ntohs(m_data.total_length);
    m_data.identification = ntohs(m_data.identification);
    // fragment_offset & flags ?
    m_data.checksum = ntohs(m_data.checksum);
}

template<>
void IP_Header::print() const
{
    std::cout << "[IP Header]\n";

    std::cout << "Version: " << +m_data.version << std::endl;
    std::cout << "Internet Header Length: " << +m_data.iheader_length << std::endl;
    std::cout << "DSCP: " << +m_data.dscp << std::endl;
    std::cout << "ECN: " << +m_data.ecn << std::endl;
    std::cout << "Total Length: " << m_data.total_length << std::endl;
    std::cout << "Identification: " << m_data.identification << std::endl;
    std::cout << "Flags: 0b" << std::bitset<3>(m_data.flags) << std::endl;
    std::cout << "Fragment Offset: " << m_data.fragment_offset << std::endl;
    std::cout << "TTL: " << +m_data.ttl << std::endl;

    std::cout << "Protocol: " << +m_data.protocol 
              << " (" << ip_proto_to_sv(IP_Protocol(m_data.protocol)) << ")"
              << std::endl;

    std::cout << std::hex << std::uppercase
              << "Checksum: 0x" << m_data.checksum
              << std::dec << std::endl;
    
    std::cout << "Source: " << ip_address_to_string(m_data.source_address) << std::endl;
    std::cout << "Destination: " << ip_address_to_string(m_data.destination_address) << std::endl;

    std::cout << std::dec;
}

IP_Packet::IP_Packet(std::vector<u8>&& bytes)
: super{std::move(bytes)} 
, m_header{offset_ptr()}
{
    m_header.into_host_endian();
    // TODO: ip header could vary in size, handle like tcp
    m_offset += sizeof(IP_Header);
}

IP_Packet::IP_Packet(super&& sup)
    : super{std::move(sup)}
    , m_header{offset_ptr()} 
{
    m_header.into_host_endian();
    // TODO: ^^ 
    m_offset += sizeof(IP_Header);
}

void IP_Packet::apply(visitors::PacketVisitor &visitor) 
{
    visitor.visit(*this);
}
