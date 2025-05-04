#ifndef REKI_ETHERNET
#define REKI_ETHERNET

#include "../defs.hpp"
#include "../packet.hpp"
#include "../packet_header.hpp"

#include <net/ethernet.h>
#include <string_view>

/// non-comprehensive due to the standard specifying any value 
/// under 1500 to be the size, see EtherType::print()
enum class EtherType : u16 {
    IPv4 = ETHERTYPE_IP,
    ARP = ETHERTYPE_ARP,
    REVARP = ETHERTYPE_REVARP,
    IPv6 = ETHERTYPE_IPV6,
    WakeOnLan = 0x0842,
    Loopback = ETHERTYPE_LOOPBACK,
};

constexpr std::string_view ethertype_to_sv(EtherType et) 
{
    switch(et) {
        case EtherType::IPv4: return "IPv4";
        case EtherType::IPv6: return "IPv6";
        case EtherType::ARP : return "ARP";
        case EtherType::REVARP : return "Reverse-ARP";
        case EtherType::WakeOnLan : return "Wake-on-Lan";
        case EtherType::Loopback: return "Loopback";
    }
    return "Unknown";
}

std::string mac_to_string(const u8 addr[6]);
std::string ethertype_to_string(EtherType et);

/// Ethernet frame header data
struct EthernetHeaderData {
    static constexpr auto destination_size = 6;
    u8 destination[destination_size];
    static constexpr auto source_size = 6;
    u8 source[source_size];
    u16 ethertype;
} __attribute__((packed));

using EthernetHeader = PacketHeader<EthernetHeaderData>;

template<>
void EthernetHeader::print() const;

template<>
void EthernetHeader::into_host_endian();

class EthernetPacket : public Packet {
private:
    EthernetHeader m_header;
public:
    using super = Packet;

    explicit EthernetPacket(std::vector<u8>&& bytes);
    explicit EthernetPacket(super&& sup);

    explicit EthernetPacket(EthernetPacket&& other)
        : super{std::move(other)}
        , m_header(other.m_header)
    {}

    virtual void apply(visitors::PacketVisitor& visitor) override;

    const EthernetHeader& eth_header() const { return this->m_header; }
};

#endif /* REKI_ETHERNET */
