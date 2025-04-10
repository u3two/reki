#include "visitors.hpp"

#include <bitset>
#include <sstream>

namespace visitors {

void ExplorerData::visit([[maybe_unused]] Packet& a) {}

void ExplorerData::visit(EthernetPacket& a) {
    ExplorerData::visit(static_cast<EthernetPacket::super&>(a));
    auto &hdr = a.eth_header().data();

    items.emplace_back(gui::ExplorerData {
        "Ethernet Header",
        {
            {"EtherType", ethertype_to_string(EtherType(hdr.ethertype))},
            {"Destination MAC", mac_to_string(hdr.destination)},
            {"Source MAC", mac_to_string(hdr.source)},
        }
    });
}

void ExplorerData::visit(IP_Packet& a) {
    ExplorerData::visit(static_cast<IP_Packet::super&>(a));

    auto &hdr = a.ip_header().data();

    std::stringstream chcksum;
    chcksum << std::hex << std::uppercase
            << "0x" << hdr.checksum
            << std::dec;

    items.emplace_back(gui::ExplorerData {
        "IP Header",
        {
            {"Version", std::to_string((u16)hdr.version) },
            {"Header Length", std::to_string((u16)hdr.iheader_length) },
            {"DSCP", std::to_string((u16)hdr.dscp) },
            {"ECN", std::to_string((u16)hdr.ecn) },
            {"Total Length", std::to_string((u16)hdr.total_length) },
            {"Identification", std::to_string((u16)hdr.identification) },
            {"Flags", std::bitset<3>(hdr.identification).to_string() },
            {"Fragment Offset", std::to_string((u16)hdr.fragment_offset) },
            {"TTL", std::to_string((u16)hdr.ttl) },
            {"Protocol", std::to_string((u16)hdr.ttl) },
            {"Checksum", chcksum.str() },
            {"Destination", ip_address_to_string(hdr.destination_address)},
            {"Source", ip_address_to_string(hdr.source_address)},
        }
    });
}

void ExplorerData::visit(TCP_Packet& a) {
    ExplorerData::visit(static_cast<TCP_Packet::super&>(a));

    auto &hdr = a.tcp_header().data();

    std::stringstream chcksum;
    chcksum << std::hex << std::uppercase
            << "0x" << hdr.checksum
            << std::dec;

    items.emplace_back(gui::ExplorerData {
        "TCP Header",
        {
            {"Source Port", std::to_string(hdr.source_port)},
            {"Destination Port", std::to_string(hdr.destination_port)},
            {"Sequence Number", std::to_string(hdr.sequence_number)},
            {"ACK Number", std::to_string(hdr.ack_number) },
            {"Flags", std::bitset<8>(hdr.flags).to_string() },
            {"Window", std::to_string(hdr.window) },
            {"Checksum", chcksum.str() },
        }
    });
}

void ExplorerData::visit(UDP_Packet& a) {
    ExplorerData::visit(static_cast<UDP_Packet::super&>(a));

    auto &hdr = a.udp_header().data();

    std::stringstream chcksum;
    chcksum << std::hex << std::uppercase
            << "0x" << hdr.checksum
            << std::dec;

    items.emplace_back(gui::ExplorerData {
        "UDP Header",
        {
            {"Source Port", std::to_string(hdr.source_port)},
            {"Destination Port", std::to_string(hdr.destination_port)},
            {"Length", std::to_string(hdr.length)},
            {"Checksum", chcksum.str() },
        }
    });
}

void ExplorerData::visit(ARP_Packet& a) {
    ExplorerData::visit(static_cast<ARP_Packet::super&>(a));
}

void ExplorerData::visit(ICMP_Packet& a) {
    ExplorerData::visit(static_cast<ICMP_Packet::super&>(a));

    auto &hdr = a.icmp_header().data();

    std::stringstream strtype;
    strtype << +hdr.type << " (" << icmp_type_to_sv(ICMP_Type(hdr.type)) << ") ";

    std::stringstream chcksum;
    chcksum << std::hex << std::uppercase
            << "0x" << hdr.checksum
            << std::dec;

    items.emplace_back(gui::ExplorerData {
        "ICMP Header",
        {
            {"Type", strtype.str()},
            {"Code", std::to_string(hdr.code)},
            {"Checksum", chcksum.str()},
        }
    });
}

} /* namespace visitors */
