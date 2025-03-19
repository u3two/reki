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
    /// pretty print header contents
    void print() const;
    /// convert relevant fields to host byte order
    void into_host_endianness();
} __attribute__((packed));

class IP_Packet : public EthernetPacket {
private:
    const IP_Header *m_header;
public:
    using super = EthernetPacket;
    IP_Packet(std::vector<u8>&& bytes);

    IP_Packet(EthernetPacket&& eth);

    virtual void apply(PacketVisitor& visitor) override;

    const IP_Header* ip_header() { return this->m_header; }
};

#endif /* REKI_IP */
