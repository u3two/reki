#ifndef REKI_ICMP_H
#define REKI_ICMP_H

#include "ip.hpp"
#include "../defs.hpp"

struct ICMP_HeaderData {
    u8 type;
    u8 code;
    u16 checksum;
    // NOTE: semi opaque, depends on type; thus interpreted as seperate bytes
    u8 rest[4];
    // TODO: the rest of the payload, not really relevant right now
} __attribute__((packed));

/// Incomplete, see RFC 792
enum class ICMP_Type {
    EchoReply,
    DestinationUnreachable = 3,
    SourceQuench = 4,
    RedirectMessage = 5,
    EchoRequest = 8,
    RouterAdvertisement = 9,
    RouterSolicitation = 10,
    TimeExceeded = 11,
    Traceroute = 30,
};

using ICMP_Header = PacketHeader<ICMP_HeaderData>;

template<>
void ICMP_Header::print() const;

template<>
void ICMP_Header::into_host_endian();

class ICMP_Packet : public IP_Packet {
private:
    ICMP_Header m_header;
public:
    using super = IP_Packet;

    explicit ICMP_Packet(std::vector<u8>&& bytes);
    explicit ICMP_Packet(super&& sup);

    explicit ICMP_Packet(ICMP_Packet&& other)
        : super{std::move(other)}
        , m_header{other.m_header}
    {}

    virtual void apply(PacketVisitor& visitor) override;

    const ICMP_Header &icmp_header() const { return this->m_header; };
};

#endif /* REKI_ICMP_H */
