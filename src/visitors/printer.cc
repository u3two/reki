#include "visitors.hpp"

#include <iostream>
#include <iomanip>

namespace visitors {

void Printer::visit(Packet& a) {
    if (this->hexdump) {
        std::cout << std::uppercase << std::hex;

        constexpr size_t cols = 20;
        size_t counter = 0;
        for (u8 c : a.bytes()) {
            std::cout << std::setw(2) << std::setfill('0')
                << +c << " ";
            counter++;
            if (counter == cols) {
                std::cout << std::endl;
                counter = 0;
            } 
        }
        std::cout << std::endl;

        std::cout << std::dec;
    }

    std::cout << "[Packet] size: " << a.bytes().size() 
              << std::endl;
}

void Printer::visit(EthernetPacket& a) {
    Printer::visit(static_cast<EthernetPacket::super&>(a));
    a.eth_header().print();
}

void Printer::visit(IP_Packet& a) {
    Printer::visit(static_cast<IP_Packet::super&>(a));
    a.ip_header().print();
}

void Printer::visit(TCP_Packet& a) {
    Printer::visit(static_cast<TCP_Packet::super&>(a));
    a.tcp_header().print();
}

void Printer::visit(UDP_Packet& a) {
    Printer::visit(static_cast<UDP_Packet::super&>(a));
    a.udp_header().print();
}

void Printer::visit(ARP_Packet& a) {
    Printer::visit(static_cast<ARP_Packet::super&>(a));
    a.arp_header().print();
}

void Printer::visit(ICMP_Packet& a) {
    Printer::visit(static_cast<ARP_Packet::super&>(a));
    a.icmp_header().print();
}

}
