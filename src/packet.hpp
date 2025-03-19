#ifndef REKI_PACKET
#define REKI_PACKET

#include "defs.hpp"

#include <vector>
#include <span>

// see: visitors/*
class PacketVisitor;

class Packet {
private:
    /// raw packet data, should never be accessed directly after intialization,
    /// for it must not reallocate.
    std::vector<u8> m_bytes;
protected:
    /// used by constructors; each constructor sets this to the beginning of the
    /// next layer's packet header
    u16 m_offset = 0;
    /// get a mutable span of the packet's raw data
    std::span<u8> bytes_mut() { return m_bytes; };
public:
    Packet(std::vector<u8>&& bytes) 
    : m_bytes{std::move(bytes)} {}

    virtual ~Packet() {};

    Packet(const Packet&) = delete;
    Packet& operator=(const Packet&) = delete;

    Packet(Packet&&) = delete;
    Packet& operator=(Packet&&) = delete;

    /// get an immutable span of the packet's raw data
    std::span<const u8> bytes() const { return m_bytes; };

    /// Apply a visitor.
    virtual void apply(PacketVisitor& visitor);
};

#endif /* REKI_PACKET */
