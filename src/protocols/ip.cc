#include "ip.hpp"
#include "../visitors/visitor.hpp"

#include <bitset>
#include <ios>
#include <iostream>

#include <arpa/inet.h>

IP_Packet::IP_Packet(std::vector<u8> bytes)
: super{bytes} 
{
    m_header = 
        reinterpret_cast<const IP_Header*>(this->bytes().data() + m_offset);
    // TODO: ip header could vary in size, handle like tcp
    m_offset += sizeof(IP_Header);
}

void IP_Header::print() const 
{
    std::cout << "[IP Header]\n";

    std::cout << "Version: " << +this->version << std::endl;
    std::cout << "Internet Header Length: " << +this->iheader_length << std::endl;
    std::cout << "DSCP: " << +this->dscp << std::endl;
    std::cout << "ECN: " << +this->ecn << std::endl;
    std::cout << "Total Length: " << ntohs(this->total_length) << std::endl;
    std::cout << "Identification: " << ntohs(this->identification) << std::endl;
    std::cout << "Flags: 0b" << std::bitset<3>(this->flags) << std::endl;
    std::cout << "Fragment Offset: " << ntohs(this->fragment_offset) << std::endl;
    std::cout << "TTL: " << +this->ttl << std::endl;

    // TODO: print proto name
    std::cout << "Protocol: " << +this->protocol << std::endl;

    std::cout << std::hex << std::uppercase
              << "Checksum: 0x" << ntohs(this->checksum)
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
