#include "tcp.hpp"
#include "../visitors/visitor.hpp"

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
    m_header.source_port = ntohs(m_header.source_port);
    m_header.destination_port = ntohs(m_header.destination_port);
    m_header.sequence_number = ntohl(m_header.sequence_number);
    m_header.ack_number = ntohl(m_header.ack_number);
    m_header.window = ntohs(m_header.window);
    m_header.checksum = ntohs(m_header.checksum);
    m_header.urgent_ptr = ntohs(m_header.urgent_ptr);
    // TODO: convert options?
}

template<>
void TCP_Header::print() const
{
    std::cout << "[TCP Header]\n";

    std::cout << "Source port: " << m_header.source_port << std::endl;
    std::cout << "Destination port: " << m_header.destination_port << std::endl;

    // TODO: has dual role
    std::cout << "Sequence number: " << m_header.sequence_number << std::endl;
    std::cout << "ACK number: " << m_header.ack_number << std::endl;

    std::cout << "Data Offset: " << +m_header.data_offset << std::endl;
    std::cout << "Reserved: " << +m_header.reserved << std::endl;
    std::cout << "Flags: " << std::bitset<8>(m_header.flags) << std::endl;

    std::cout << "Window: " << m_header.window << std::endl;
    std::cout << "Checksum: " << m_header.checksum << std::endl;
    std::cout << "Urgent Pointer: " << m_header.urgent_ptr << std::endl;
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

void TCP_Packet::apply(PacketVisitor &visitor) 
{
    visitor.visit(*this);
}
