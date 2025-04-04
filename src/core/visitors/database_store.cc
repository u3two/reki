#include "visitors.hpp"

void DatabaseStore::visit(Packet& a) {
    // m_sql << "witam";
}

void DatabaseStore::visit(EthernetPacket& a) {
}

void DatabaseStore::visit(IP_Packet& a) {
}

void DatabaseStore::visit(TCP_Packet& a) {
}

void DatabaseStore::visit(UDP_Packet& a) {
}

void DatabaseStore::visit(ARP_Packet& a) {
}

void DatabaseStore::visit(ICMP_Packet& a) {
}
