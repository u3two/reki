#ifndef REKI_IP
#define REKI_IP

#include "ethernet.hpp"
#include "../packet.hpp"

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

/// IPv4 packet header
/// https://datatracker.ietf.org/doc/html/rfc791 Section 3.1
struct IP_Header {
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

    /// (copy) construct an IP_Header from raw data
    /// assumes *data is >= sizeof(IP_Header)
    IP_Header(const u8 *data) {
        *this = *reinterpret_cast<const IP_Header*>(data);
    }

    /// convert relevant fields to host byte order
    void into_host_endianness();
    /// pretty print header contents
    void print() const;
} __attribute__((packed));

class IP_Packet : public EthernetPacket {
private:
    IP_Header m_header;
public:
    using super = EthernetPacket;

    IP_Packet(std::vector<u8>&& bytes);

    IP_Packet(IP_Packet&& other)
        : super(std::move(other))
        , m_header(other.m_header)
    {}

    IP_Packet(super&& sup)
        : super{std::move(sup)}
        , m_header{sup.offset_ptr()} 
    {}

    virtual void apply(PacketVisitor& visitor) override;

    const IP_Header& ip_header() { return this->m_header; }
};

#endif /* REKI_IP */
