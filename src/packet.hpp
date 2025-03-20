#ifndef REKI_PACKET
#define REKI_PACKET

#include "defs.hpp"

#include <vector>

// see: visitors/*
class PacketVisitor;

class Packet {
private:
    /// raw packet data, should never be accessed directly after intialization,
    /// for it must not reallocate.
    const std::vector<u8> m_bytes;
protected:
    /// used by constructors; each constructor sets this to the beginning of the
    /// next layer's packet header
    u16 m_offset = 0;
public:
    Packet(std::vector<u8>&& bytes) 
    : m_bytes{std::move(bytes)} {}

    virtual ~Packet() {};

    // disallow copy operations
    Packet(const Packet&) = delete;
    Packet& operator=(const Packet&) = delete;

    // disallow move assignment
    Packet& operator=(Packet&&) = delete;

    // .. but _allow_ move construction
    Packet(Packet&& other)
        : m_bytes(std::move(other.m_bytes)) 
        , m_offset(other.m_offset)
    {}

    /// get an immutable span of the packet's raw data
    const std::vector<u8>& bytes() const { return m_bytes; };

    /// get data ptr to current offset
    const u8 *offset_ptr() const { return m_bytes.data() + m_offset; }

    /// Apply a visitor.
    virtual void apply(PacketVisitor& visitor);

    /// "upgrade" this packet type to the next layer down.
    /// .second is true if an upgrade took place, otherwise the Packet is
    /// returned unchanged.
    // virtual std::pair<std::unique_ptr<Packet>, bool> upgrade() &&;
};

#endif /* REKI_PACKET */
