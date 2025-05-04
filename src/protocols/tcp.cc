#include "tcp.hpp"
#include "../visitors/visitors.hpp"

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <bitset>

std::vector<u8> TCP_HeaderData::options() const {
    std::vector<u8> ret;
    const auto options_size = (this->data_offset - 5) * 32;
    ret.resize(options_size);
    std::memcpy(ret.data(), this + sizeof(TCP_Header), options_size);
    return ret;
}

template<>
void TCP_Header::into_host_endian() 
{
    m_data.source_port = ntohs(m_data.source_port);
    m_data.destination_port = ntohs(m_data.destination_port);
    m_data.sequence_number = ntohl(m_data.sequence_number);
    m_data.ack_number = ntohl(m_data.ack_number);
    m_data.window = ntohs(m_data.window);
    m_data.checksum = ntohs(m_data.checksum);
    m_data.urgent_ptr = ntohs(m_data.urgent_ptr);
    // TODO: convert options?
}

template<>
void TCP_Header::print() const
{
    std::cout << "[TCP Header]\n";

    std::cout << "Source port: " << m_data.source_port << std::endl;
    std::cout << "Destination port: " << m_data.destination_port << std::endl;

    // TODO: has dual role
    std::cout << "Sequence number: " << m_data.sequence_number << std::endl;
    std::cout << "ACK number: " << m_data.ack_number << std::endl;

    std::cout << "Data Offset: " << +m_data.data_offset << std::endl;
    std::cout << "Reserved: " << +m_data.reserved << std::endl;
    std::cout << "Flags: " << std::bitset<8>(m_data.flags) << std::endl;

    std::cout << "Window: " << m_data.window << std::endl;
    std::cout << "Checksum: " << m_data.checksum << std::endl;
    std::cout << "Urgent Pointer: " << m_data.urgent_ptr << std::endl;
    // TODO: options
}

TCP_Packet::TCP_Packet(std::vector<u8>&& bytes) 
: super{std::move(bytes)}
, m_header{offset_ptr()}
{
    m_header.into_host_endian();
    m_offset += sizeof(TCP_Header) + m_header.data().options().size();
}

TCP_Packet::TCP_Packet(super&& sup)
    : super{std::move(sup)}
    , m_header{offset_ptr()} 
{
    m_header.into_host_endian();
}

void TCP_Packet::apply(visitors::PacketVisitor &visitor) 
{
    visitor.visit(*this);
}
