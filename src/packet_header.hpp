#ifndef REKI_PACKET_HEADER
#define REKI_PACKET_HEADER

#include "defs.hpp"

template<typename header_data_t>
class PacketHeader {
private:
    header_data_t m_data;
public:
    explicit PacketHeader(const u8 *data) 
    : m_data { *reinterpret_cast<const header_data_t*>(data) } {}

    /// convert relevant fields to host byte order
    void into_host_endian() { 
        static_assert(0, "use an implemented specialization of PacketHeader"); 
    };

    /// pretty print header data
    void print() const { 
        static_assert(0, "use an implemented specialization of PacketHeader"); 
    };

    const header_data_t& data() const { return m_data; }
};

#endif /* REKI_PACKET_HEADER */
