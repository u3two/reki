#include "visitors.hpp"

#include <sstream>

namespace visitors {

void ListingData::visit([[maybe_unused]] Packet& a) {
    m_listing.info = "BASE/UNIMPLEMENTED";
    m_listing.rgb = {50, 50, 50, 255};
}

void ListingData::visit(EthernetPacket& a) {
    std::ostringstream oss;
    oss << "[Ethernet] ethertype: " << a.eth_header().data().ethertype;

    m_listing.info = oss.str();
    m_listing.rgb = {200, 140, 64, 255}; // orange-ish
}

void ListingData::visit(IP_Packet& a) {
    std::ostringstream oss;
    oss << "[IP] protocol: " << a.ip_header().data().protocol;

    m_listing.info = oss.str();
    m_listing.rgb = {80, 130, 170, 255}; // blue-ish
}

void ListingData::visit(TCP_Packet& a) {
    std::ostringstream oss;
    oss << "[TCP] ";

    auto &iphdr = a.ip_header().data();

    oss << ip_address_to_string(iphdr.source_address)
        << " -> "
        << ip_address_to_string(iphdr.destination_address);

    m_listing.info = oss.str();
    m_listing.rgb = {200, 60, 110, 255}; // pink-red-ish
}

void ListingData::visit(UDP_Packet& a) {
    std::ostringstream oss;
    oss << "[UDP] ";

    auto &iphdr = a.ip_header().data();

    oss << ip_address_to_string(iphdr.source_address)
        << " -> "
        << ip_address_to_string(iphdr.destination_address);

    m_listing.info = oss.str();
    m_listing.rgb = {60, 170, 130, 255}; // blue-green-ish
}

void ListingData::visit(ARP_Packet& a) {
    std::ostringstream oss;
    oss << "[ARP] protocol: " << a.arp_header().data().protocol_type;

    m_listing.info = oss.str();
    m_listing.rgb = {170, 180, 130, 255}; // very-worn-out-yellow-ish
}

void ListingData::visit(ICMP_Packet& a) {
    std::ostringstream oss;
    oss << "[ICMP] protocol: " << +a.icmp_header().data().type;

    m_listing.info = oss.str();
    m_listing.rgb = {35, 190, 168, 255}; // light-blue-or-cyan-?-ish
}

}
