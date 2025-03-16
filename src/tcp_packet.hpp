#ifndef REKI_TCP_PACKET
#define REKI_TCP_PACKET

#include "ip_packet.hpp"

class TCP_Packet : public IP_Packet {
private:
    using super = IP_Packet;
public:
    TCP_Packet(std::vector<u8> data) 
    : super{data} {}
};

#endif /* REKI_TCP_PACKET */
