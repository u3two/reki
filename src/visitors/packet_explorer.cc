#include "visitors.hpp"

void PacketGUIExplorer::visit([[maybe_unused]] Packet& a) {}

void PacketGUIExplorer::visit(EthernetPacket& a) {
    PacketGUIExplorer::visit(static_cast<EthernetPacket::super&>(a));
    auto hdr = a.eth_header().data();
    items.emplace_back(ExplorerItem {
        "Ethernet Header",
        {
            {"EtherType", ethertype_to_string(EtherType(hdr.ethertype))},
            {"Destination MAC", mac_to_string(hdr.destination)},
            {"Source MAC", mac_to_string(hdr.source)},
        }
    });
}

void PacketGUIExplorer::visit(IP_Packet& a) {
    PacketGUIExplorer::visit(static_cast<IP_Packet::super&>(a));
    auto hdr = a.ip_header().data();
    items.emplace_back(ExplorerItem {
        "IP Header",
        {
            {"Destination", ip_address_to_string(hdr.destination_address)},
            {"Source", ip_address_to_string(hdr.source_address)},
        }
    });
}

void PacketGUIExplorer::visit(TCP_Packet& a) {
}

void PacketGUIExplorer::visit(UDP_Packet& a) {
}

void PacketGUIExplorer::visit(ARP_Packet& a) {
}
