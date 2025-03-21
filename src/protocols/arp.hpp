#ifndef REKI_ARP
#define REKI_ARP

#include "../defs.hpp"
#include "ethernet.hpp"

struct ARP_HeaderData {
    u16 hardware_type;
    u16 protocol_type;
    u8 hardware_length;
    u8 protocol_length;
    u16 operation;
    /// length dictated by hardware_length
    std::vector<u8> sender_hardware_address(const u8 *address_fields_start) const;
    /// length dictated by protocol_length
    std::vector<u8> sender_protocol_address(const u8 *address_fields_start) const;
    /// length dictated by hardware_length
    std::vector<u8> target_hardware_address(const u8 *address_fields_start) const;
    /// length dictated by protocol_length
    std::vector<u8> target_protocol_address(const u8 *address_fields_start) const;
} __attribute__((packed));

template<>
class PacketHeader<ARP_HeaderData> {
private:
    ARP_HeaderData m_header;
    const u8 *m_address_fields_start;
public:
    explicit PacketHeader(const u8 *data) 
        : m_header { *reinterpret_cast<const ARP_HeaderData*>(data) } 
        , m_address_fields_start { data + sizeof(ARP_HeaderData) }
    {}

    void into_host_endian();
    void print() const;

    const ARP_HeaderData& data() const { return m_header; }
};

using ARP_Header = PacketHeader<ARP_HeaderData>;

class ARP_Packet : public EthernetPacket {
private:
    ARP_Header m_header;
public:
    using super = EthernetPacket;

    explicit ARP_Packet(std::vector<u8>&& bytes);
    explicit ARP_Packet(super&& sup);

    virtual void apply(PacketVisitor& visitor) override;

    const ARP_Header& arp_header() const { return this->m_header; }
};

#endif /* REKI_ARP */
