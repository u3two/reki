#include "visitors.hpp"

#include <sstream>

void PacketGUIExplorer::visit(Packet& a) {
}

void PacketGUIExplorer::visit(EthernetPacket& a) {
}

void PacketGUIExplorer::visit(IP_Packet& a) {
}

void PacketGUIExplorer::visit(TCP_Packet& a) {
}

void PacketGUIExplorer::visit(UDP_Packet& a) {
}

void PacketGUIExplorer::visit(ARP_Packet& a) {
}
