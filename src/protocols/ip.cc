#include "ip.hpp"
#include "../visitors/visitor.hpp"

#include <bitset>
#include <ios>
#include <iostream>

#include <arpa/inet.h>

IP_Packet::IP_Packet(std::vector<u8>&& bytes)
: super{std::move(bytes)} 
, m_header{offset_ptr()}
{
    m_header.into_host_endianness();
    // TODO: ip header could vary in size, handle like tcp
    m_offset += sizeof(IP_Header);
}

void IP_Header::into_host_endianness()
{
    this->total_length = ntohs(this->total_length);
    this->identification = ntohs(this->identification);
    // fragment_offset & flags ?
    this->checksum = ntohs(this->checksum);
}

void IP_Header::print() const 
{
    std::cout << "[IP Header]\n";

    std::cout << "Version: " << +this->version << std::endl;
    std::cout << "Internet Header Length: " << +this->iheader_length << std::endl;
    std::cout << "DSCP: " << +this->dscp << std::endl;
    std::cout << "ECN: " << +this->ecn << std::endl;
    std::cout << "Total Length: " << this->total_length << std::endl;
    std::cout << "Identification: " << this->identification << std::endl;
    std::cout << "Flags: 0b" << std::bitset<3>(this->flags) << std::endl;
    std::cout << "Fragment Offset: " << this->fragment_offset << std::endl;
    std::cout << "TTL: " << +this->ttl << std::endl;

    std::cout << "Protocol: " << +this->protocol 
              << " (" << ip_proto_to_sv(IP_Protocol(this->protocol)) << ")"
              << std::endl;

    std::cout << std::hex << std::uppercase
              << "Checksum: 0x" << this->checksum
              << std::dec << std::endl;
    
    std::cout << "Source: ";
    for (u8 i = 0; i < sizeof(source_address); i++)
        std::cout << +this->source_address[i]
                  << ((i != sizeof(source_address) - 1) ? "." : "");
    std::cout << std::endl;

    std::cout << "Destination: ";
    for (u8 i = 0; i < sizeof(destination_address); i++)
        std::cout << +this->destination_address[i]
                  << ((i != sizeof(destination_address) - 1) ? "." : "");
    std::cout << std::endl;

    std::cout << std::dec;
}

void IP_Packet::apply(PacketVisitor &visitor) 
{
    visitor.visit(*this);
}
