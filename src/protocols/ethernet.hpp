#ifndef REKI_ETHERNET
#define REKI_ETHERNET

#include "../defs.hpp"
#include "../packet.hpp"

#include <net/ethernet.h>

/// Non-comprehensive due to the standard specifying  any value 
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

/// Ethernet frame header data
struct EthernetHeader {
    static constexpr auto destination_size = 6;
    u8 destination[destination_size];
    static constexpr auto source_size = 6;
    u8 source[source_size];
    u16 ethertype;

    /// (copy) construct an EthernetHeader from raw data
    /// assumes *data is >= sizeof(EthernetHeader)
    EthernetHeader(const u8 *data) {
        *this = *reinterpret_cast<const EthernetHeader*>(data);
    }

    /// convert relevant fields to host byte order
    void into_host_endianness();
    /// pretty print header data
    void print() const;
} __attribute__((packed));

class EthernetPacket : public Packet {
private:
    EthernetHeader m_header;
public:
    using super = Packet;

    EthernetPacket(std::vector<u8>&& bytes);

    EthernetPacket(EthernetPacket&& other)
        : super{std::move(other)}
        , m_header(other.m_header)
    {}

    EthernetPacket(super&& sup)
        : super{std::move(sup)}
        , m_header{sup.offset_ptr()} 
    {}

    virtual void apply(PacketVisitor& visitor) override;

    const EthernetHeader& eth_header() { return this->m_header; }
};

#endif /* REKI_ETHERNET */
