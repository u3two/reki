#ifndef REKI_PACKET
#define REKI_PACKET

#include "defs.hpp"
#include "ethernet.hpp"

#include <vector>

/// Packet base class encapsulating the ethernet frame header. Classes representing specific
/// protocols can further derive from this class (eg. IP_Packet)
class Packet {
private:
    const EthernetHeader *m_etherhdr;
protected:
    // stores the raw packet data
    const std::vector<u8> m_data;

    // used by constructors; each constructor sets this to the beginning of the
    // next layer's packet header
    u16 m_offset;
public:
    /// TODO: disallow this (?) and define a move constructor
    Packet(std::vector<u8> data);
    virtual ~Packet() {};

    virtual void print() const;
    virtual void print_data() const;

    u32 length() const { return this->m_data.size(); }
};

#endif /* REKI_PACKET */
