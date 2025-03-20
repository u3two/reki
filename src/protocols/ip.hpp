#ifndef REKI_IP
#define REKI_IP

#include "../packet_header.hpp"
#include "ethernet.hpp"

// https://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml
enum class IP_Protocol {
    ICMP = 1,
    TCP = 6,
    UDP = 17
};

constexpr std::string_view ip_proto_to_sv(IP_Protocol ip)
{
    switch(ip) {
        case IP_Protocol::TCP: return "TCP";
        case IP_Protocol::UDP: return "UDP";
        case IP_Protocol::ICMP: return "ICMP";
    }
    return "Unknown";
}

/// IPv4 packet header data
/// https://datatracker.ietf.org/doc/html/rfc791 Section 3.1
struct IP_HeaderData {
    u8 version : 4;
    u8 iheader_length : 4;
    u8 dscp : 6;
    u8 ecn : 2;
    u16 total_length;
    u16 identification;
    u16 flags : 3;
    u16 fragment_offset : 13;
    u8 ttl;
    u8 protocol;
    u16 checksum;
    u8 source_address[4];
    u8 destination_address[4];
} __attribute__((packed));

using IP_Header = PacketHeader<IP_HeaderData>;

template<>
void IP_Header::print() const;

template<>
void IP_Header::into_host_endian();

class IP_Packet : public EthernetPacket {
private:
    IP_Header m_header;
public:
    using super = EthernetPacket;

    IP_Packet(std::vector<u8>&& bytes);
    IP_Packet(super&& sup);

    IP_Packet(IP_Packet&& other)
        : super{std::move(other)}
        , m_header{other.m_header}
    {}

    virtual void apply(PacketVisitor& visitor) override;

    const IP_Header& ip_header() { return this->m_header; }
};

#endif /* REKI_IP */
