#include "visitors.hpp"

#define TRY_MATCH(str) do { \
    if (this->filter_text == str) \
        this->matched = true; \
    else \
        this->matched = false; \
} while(0)

namespace visitors {

void Filter::visit([[maybe_unused]] Packet& a) {
    this->matched = false;
}

void Filter::visit([[maybe_unused]] EthernetPacket& a) {
    TRY_MATCH("ethernet");
}

void Filter::visit([[maybe_unused]] IP_Packet& a) {
    TRY_MATCH("ip");
}

void Filter::visit([[maybe_unused]] TCP_Packet& a) {
    TRY_MATCH("tcp");
}

void Filter::visit([[maybe_unused]] UDP_Packet& a) {
    TRY_MATCH("udp");
}

void Filter::visit([[maybe_unused]] ARP_Packet& a) {
    TRY_MATCH("arp");
}

void Filter::visit([[maybe_unused]] ICMP_Packet& a) {
    TRY_MATCH("icmp");
}

}
