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

/// Ethernet frame header data
struct EthernetHeader {
    static constexpr auto destination_size = 6;
    u8 destination[destination_size];
    static constexpr auto source_size = 6;
    u8 source[source_size];
    u16 ethertype;
    /// convert relevant fields to host byte order
    void into_host_endianness();
    /// pretty print header data
    void print() const;
} __attribute__((packed));

class EthernetPacket : public Packet {
private:
    const EthernetHeader *m_header;
public:
    using super = Packet;
    EthernetPacket(std::vector<u8>&& bytes);

    virtual void apply(PacketVisitor& visitor) override;

    const EthernetHeader* eth_header() { return this->m_header; }
};

#endif /* REKI_ETHERNET */
