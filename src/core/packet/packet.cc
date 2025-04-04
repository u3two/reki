#include "packet.hpp"

#include "visitors/visitors.hpp"

void Packet::apply(PacketVisitor &visitor)
{
    visitor.visit(*this);
}
