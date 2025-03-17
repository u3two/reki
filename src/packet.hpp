#ifndef REKI_PACKET
#define REKI_PACKET

#include "defs.hpp"

#include <vector>

// see: visitors/*
class PacketVisitor;

class Packet {
private:
    // raw packet data
    const std::vector<u8> m_bytes;
protected:
    // used by constructors; each constructor sets this to the beginning of the
    // next layer's packet header
    u16 m_offset = 0;
public:
    Packet(std::vector<u8> bytes) 
    : m_bytes{bytes} {}

    virtual ~Packet() {};

    const std::vector<u8>& bytes() const { return m_bytes; };

    /// Apply a visitor.
    virtual void apply(PacketVisitor& visitor);
};

#endif /* REKI_PACKET */
