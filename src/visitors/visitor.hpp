#ifndef REKI_VISITOR
#define REKI_VISITOR

#include "../packet.hpp"
#include "../protocols/ethernet.hpp"
#include "../protocols/ip.hpp"
#include "../protocols/tcp.hpp"
#include "../protocols/udp.hpp"
#include "../protocols/arp.hpp"

class PacketVisitor {
public:
    virtual ~PacketVisitor() {}

    virtual void visit(Packet& a) = 0;
    virtual void visit(EthernetPacket& a) = 0;
    virtual void visit(IP_Packet& a) = 0;
    virtual void visit(TCP_Packet& a) = 0;
    virtual void visit(UDP_Packet& a) = 0;
    virtual void visit(ARP_Packet& a) = 0;
};

class PacketPrinter final : public PacketVisitor {
public:
    bool hexdump = false;

    void visit(Packet& a) override;
    void visit(EthernetPacket& a) override;
    void visit(IP_Packet& a) override;
    void visit(TCP_Packet& a) override;
    void visit(UDP_Packet& a) override;
    void visit(ARP_Packet& a) override;
};

#endif /* REKI_VISITOR */
