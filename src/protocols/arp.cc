#include "arp.hpp"
#include "../visitors/visitors.hpp"

#include <iostream>
#include <iomanip>
#include <arpa/inet.h>

#include <cassert>
#include <cstring>
#include <sstream>

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

void arp_address_stream_format(std::ostream &stream, const std::vector<u8>& vec)
{
    switch(vec.size()) {
        case 4:
            for (size_t i = 0; i < vec.size(); i++)
                stream << +vec[i]
                    << (i != vec.size()-1 ? "." : "");
            break;
        default:
            stream << std::hex;
            for (size_t i = 0; i < vec.size(); i++)
                stream << std::setw(2) << std::setfill('0') 
                    << +vec[i]
                    << (i != vec.size()-1 ? ":" : "");
            stream << std::dec;
    }
}

std::string arp_address_to_string(const std::vector<u8>& vec)
{
    std::stringstream sstream;

    arp_address_stream_format(sstream, vec);

    return sstream.str();
}

void ARP_Header::print() const
{
    std::cout << "[ARP Header]\n";

    std::cout << "Hardware Type: " << m_header.hardware_type << std::endl;
    std::cout << "Protocol Type: " << m_header.protocol_type << std::endl;
    std::cout << "Hardware Length: " << +m_header.hardware_length << std::endl;
    std::cout << "Protocol Length: " << +m_header.protocol_length << std::endl;
    std::cout << "Operation: " << +m_header.operation << std::endl;

    std::cout << "Sender Hardware Address: ";
    arp_address_stream_format(std::cout, this->sender_hardware_address());
    std::cout << std::endl;

    std::cout << "Sender Protocol Address: ";
    arp_address_stream_format(std::cout, this->sender_protocol_address());
    std::cout << std::endl;

    std::cout << "Target Hardware Address: ";
    arp_address_stream_format(std::cout, this->target_hardware_address());
    std::cout << std::endl;

    std::cout << "Target Protocol Address: ";
    arp_address_stream_format(std::cout, this->target_protocol_address());
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

void ARP_Packet::apply(visitors::PacketVisitor &visitor) 
{
    visitor.visit(*this);
}
