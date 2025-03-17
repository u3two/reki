#ifndef REKI_VISITOR
#define REKI_VISITOR

#include "../packet.hpp"

class PacketVisitor {
public:
    virtual ~PacketVisitor() {}

    virtual void visit(Packet &a) = 0;
};

#endif /* REKI_VISITOR */
