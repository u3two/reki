#include "packet.hpp"

#include "visitors/visitors.hpp"

void Packet::apply(visitors::PacketVisitor &visitor)
{
    visitor.visit(*this);
}
