#ifndef REKI_UDP_PACKET
#define REKI_UDP_PACKET

#include "ip.hpp"
#include "../packet_header.hpp"

struct UDP_HeaderData {
    u16 source_port;
    u16 destination_port;
    u16 length;
    u16 checksum; // TODO: this field might be optional in ipv4 packets?
} __attribute__((packed));

using UDP_Header = PacketHeader<UDP_HeaderData>;

template<>
void UDP_Header::print() const;
template<>
void UDP_Header::into_host_endian();

class UDP_Packet : public IP_Packet {
private:
    UDP_Header m_header;
public:
    using super = IP_Packet;

    explicit UDP_Packet(std::vector<u8>&& bytes);
    explicit UDP_Packet(super&& sup);

    virtual void apply(visitors::PacketVisitor& visitor) override;

    const UDP_Header& udp_header() const { return this->m_header; }
};

#endif /* REKI_UDP_PACKET */
