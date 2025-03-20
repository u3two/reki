#include "packet.hpp"

#include "visitors/visitor.hpp"

void Packet::apply(PacketVisitor &visitor)
{
    visitor.visit(*this);
}

// std::pair<std::unique_ptr<Packet>, bool> Packet::upgrade() &&
// {
//     return {std::make_unique<EthernetPacket>(std::move(*this)), true};
// }
