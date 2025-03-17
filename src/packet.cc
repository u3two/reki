#include "packet.hpp"

#include "visitors/visitor.hpp"

void Packet::apply(PacketVisitor &visitor)
{
    visitor.visit(*this);
}
