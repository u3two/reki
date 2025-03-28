#include "visitors.hpp"

#include <sstream>

void PacketGUIListing::visit(Packet& a) {
    m_listing.text = "BASE/UNIMPLEMENTED";
    m_listing.rgb = {50, 50, 50};
}

void PacketGUIListing::visit(EthernetPacket& a) {
    std::ostringstream oss;
    oss << "[Ethernet] ethertype: " << a.eth_header().data().ethertype;

    m_listing.text = oss.str();
    m_listing.rgb = {200, 140, 64}; // orange-ish
}

void PacketGUIListing::visit(IP_Packet& a) {
    std::ostringstream oss;
    oss << "[IP] protocol: " << a.ip_header().data().protocol;

    m_listing.text = oss.str();
    m_listing.rgb = {80, 130, 170}; // blue-ish
}

void PacketGUIListing::visit(TCP_Packet& a) {
    std::ostringstream oss;
    oss << "[TCP] ";

    auto &iphdr = a.ip_header().data();

    oss << ip_address_to_string(iphdr.source_address)
        << " -> "
        << ip_address_to_string(iphdr.destination_address);

    m_listing.text = oss.str();
    m_listing.rgb = {200, 60, 110}; // pink-red-ish
}

void PacketGUIListing::visit(UDP_Packet& a) {
    std::ostringstream oss;
    oss << "[UDP] ";

    auto &iphdr = a.ip_header().data();

    oss << ip_address_to_string(iphdr.source_address)
        << " -> "
        << ip_address_to_string(iphdr.destination_address);

    m_listing.text = oss.str();
    m_listing.rgb = {60, 170, 130}; // blue-green-ish
}

void PacketGUIListing::visit(ARP_Packet& a) {
    std::ostringstream oss;
    oss << "[ARP] protocol: " << a.arp_header().data().protocol_type;

    m_listing.text = oss.str();
    m_listing.rgb = {170, 180, 130}; // very-worn-out-yellow-ish
}
