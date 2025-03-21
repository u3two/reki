#include "arp.hpp"
#include "../visitors/visitor.hpp"

#include <iostream>
#include <iomanip>
#include <arpa/inet.h>

#include <cassert>
#include <cstring>

std::vector<u8> ARP_HeaderData::sender_hardware_address(const u8 *address_fields_start) const
{
    std::vector<u8> ret;
    ret.resize(this->hardware_length);
    memcpy(ret.data(), address_fields_start, ret.size());
    return ret;
}

std::vector<u8> ARP_HeaderData::sender_protocol_address(const u8 *address_fields_start) const
{
    std::vector<u8> ret;
    ret.resize(this->protocol_length);

    auto offset = this->hardware_length;

    memcpy(ret.data(), address_fields_start + offset, ret.size());
    return ret;
}

std::vector<u8> ARP_HeaderData::target_hardware_address(const u8 *address_fields_start) const
{
    std::vector<u8> ret;
    ret.resize(this->protocol_length);

    auto offset = this->hardware_length + this->protocol_length;

    memcpy(ret.data(), address_fields_start + offset, ret.size());
    return ret;
}

std::vector<u8> ARP_HeaderData::target_protocol_address(const u8 *address_fields_start) const
{
    std::vector<u8> ret;
    ret.resize(this->protocol_length);

    auto offset = 2*this->hardware_length + this->protocol_length;

    memcpy(ret.data(), address_fields_start + offset, ret.size());
    return ret;
}

void ARP_Header::print() const
{
    std::cout << "[ARP Header]\n";

    std::cout << "Hardware Type: " << m_header.hardware_type << std::endl;
    std::cout << "Protocol Type: " << m_header.protocol_type << std::endl;
    std::cout << "Hardware Length: " << +m_header.hardware_length << std::endl;
    std::cout << "Protocol Length: " << +m_header.protocol_length << std::endl;
    std::cout << "Operation: " << +m_header.operation << std::endl;

    auto print_address = [](const std::vector<u8>& vec) {
        switch(vec.size()) {
            case 4:
                for (size_t i = 0; i < vec.size(); i++)
                    std::cout << +vec[i]
                              << (i != vec.size()-1 ? "." : "");
                break;
            default:
                std::cout << std::hex;
                for (size_t i = 0; i < vec.size(); i++)
                    std::cout << std::setw(2) << std::setfill('0') 
                              << +vec[i]
                              << (i != vec.size()-1 ? ":" : "");
                std::cout << std::dec;
        }
    };

    std::cout << "Sender Hardware Address: ";
    print_address(m_header.sender_hardware_address(this->m_address_fields_start));
    std::cout << std::endl;

    std::cout << "Sender Protocol Address: ";
    print_address(m_header.sender_protocol_address(this->m_address_fields_start));
    std::cout << std::endl;

    std::cout << "Target Hardware Address: ";
    print_address(m_header.target_hardware_address(this->m_address_fields_start));
    std::cout << std::endl;

    std::cout << "Target Protocol Address: ";
    print_address(m_header.target_protocol_address(this->m_address_fields_start));
    std::cout << std::endl;
}

void ARP_Header::into_host_endian()
{
    m_header.hardware_type = ntohs(m_header.hardware_type);
    m_header.protocol_type = ntohs(m_header.protocol_type);
    m_header.operation = ntohs(m_header.operation);
}

ARP_Packet::ARP_Packet(std::vector<u8>&& bytes) 
: super{std::move(bytes)}
, m_header{offset_ptr()}
{
    m_header.into_host_endian();
    m_offset += sizeof(ARP_Header) 
             + 2*m_header.data().hardware_length
             + 2*m_header.data().protocol_length;
}

ARP_Packet::ARP_Packet(super&& sup)
    : super{std::move(sup)}
    , m_header{offset_ptr()} 
{
    m_header.into_host_endian();
}

void ARP_Packet::apply(PacketVisitor &visitor) 
{
    visitor.visit(*this);
}
