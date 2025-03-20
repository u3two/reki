#ifndef REKI_PACKET_HEADER
#define REKI_PACKET_HEADER

#include "defs.hpp"

template<typename header_t>
class PacketHeader {
private:
    header_t m_header;
public:
    PacketHeader(const u8 *data) {
        m_header = *reinterpret_cast<const header_t*>(data);
    }

    /// convert relevant fields to host byte order
    void into_host_endian() { 
        static_assert(0, "use an implemented specialization of PacketHeader"); 
    };

    /// pretty print header data
    void print() const { 
        static_assert(0, "use an implemented specialization of PacketHeader"); 
    };

    const header_t& data() const { return m_header; }
};

#endif /* REKI_PACKET_HEADER */
