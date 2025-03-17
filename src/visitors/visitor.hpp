#ifndef REKI_VISITOR
#define REKI_VISITOR

#include "../packet.hpp"
#include "../protocols/ethernet.hpp"

class PacketVisitor {
public:
    virtual ~PacketVisitor() {}

    virtual void visit(Packet &a) = 0;
    virtual void visit(EthernetPacket &a) = 0;
};

class PacketPrinter : public PacketVisitor {
public:
    bool hexdump = false;

    virtual ~PacketPrinter() {};

    void visit(Packet &a) override final;
    void visit(EthernetPacket &a) override final;
};

#endif /* REKI_VISITOR */
