#ifndef REKI_TCP_PACKET
#define REKI_TCP_PACKET

#include "ip.hpp"
#include "../packet_header.hpp"

struct TCP_HeaderData {
    u16 source_port;
    u16 destination_port;
    u32 sequence_number;
    u32 ack_number;
    u8 data_offset : 4;
    u8 reserved : 4;
    u8 flags;
    u16 window;
    u16 checksum;
    u16 urgent_ptr;
    // + options of variable length
    std::vector<u8> options() const;
} __attribute__((packed));

using TCP_Header = PacketHeader<TCP_HeaderData>;

template<>
void TCP_Header::print() const;

template<>
void TCP_Header::into_host_endian();

class TCP_Packet : public IP_Packet {
private:
    TCP_Header m_header;
public:
    using super = IP_Packet;

    explicit TCP_Packet(std::vector<u8>&& bytes);
    explicit TCP_Packet(super&& sup);

    virtual void apply(PacketVisitor& visitor) override;

    const TCP_Header& tcp_header() const { return this->m_header; }
};

#endif /* REKI_TCP_PACKET */
