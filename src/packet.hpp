#ifndef REKI_PACKET
#define REKI_PACKET

#include "defs.hpp"

#include <ctime>
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
    /// UNIX timestamp of the arrival date
    u64 m_arrival_time;
public:
    explicit Packet(std::vector<u8>&& bytes) 
    : m_bytes{std::move(bytes)} 
    , m_arrival_time(std::time(NULL))
    {}

    virtual ~Packet() {};

    // disallow copy operations
    Packet(const Packet&) = delete;
    Packet& operator=(const Packet&) = delete;

    // disallow move assignment
    Packet& operator=(Packet&&) = delete;

    // .. but _allow_ move construction
    explicit Packet(Packet&& other)
        : m_bytes(std::move(other.m_bytes)) 
        , m_offset(other.m_offset)
        , m_arrival_time(std::time(NULL))
    {}

    /// get an immutable span of the packet's raw data
    const std::vector<u8>& bytes() const { return m_bytes; }

    /// get data ptr to current offset
    const u8 *offset_ptr() const { return m_bytes.data() + m_offset; }

    u64 arrival_time() const { return m_arrival_time; }

    /// Apply a visitor.
    virtual void apply(PacketVisitor& visitor);
};

#endif /* REKI_PACKET */
