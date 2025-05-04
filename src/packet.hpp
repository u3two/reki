#ifndef REKI_PACKET
#define REKI_PACKET

#include "defs.hpp"

#include <ctime>
#include <vector>

namespace visitors {
// see: visitors/*
class PacketVisitor;
}

class Packet {
private:
    /// raw packet data, should never be accessed directly after intialization,
    /// for it must not reallocate.
    const std::vector<u8> m_bytes;
    /// UNIX timestamp of the arrival date
    u64 m_arrival_time;
protected:
    /// used by constructors; each constructor sets this to the beginning of the
    /// next layer's packet header
    u16 m_offset = 0;
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
        , m_arrival_time(std::time(NULL))
        , m_offset(other.m_offset)
    {}

    /// get an immutable reference to the packet's raw data
    const std::vector<u8>& bytes() const { return m_bytes; }

    /// get data ptr to current offset
    const u8 *offset_ptr() const { return m_bytes.data() + m_offset; }

    /// get the packet arrival time (in seconds since the unix epoch)
    u64 arrival_time() const { return m_arrival_time; }

    /// get the combined size of all the headers, in bytes
    u32 header_size() const { return m_offset; }

    /// apply a visitor
    virtual void apply(visitors::PacketVisitor& visitor);
};

#endif /* REKI_PACKET */
