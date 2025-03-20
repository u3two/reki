#include "visitor.hpp"

#include <iostream>
#include <iomanip>

void PacketPrinter::visit(Packet &a) {
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

void PacketPrinter::visit(EthernetPacket &a) {
    PacketPrinter::visit(static_cast<EthernetPacket::super&>(a));
    a.eth_header().print();
}

void PacketPrinter::visit(IP_Packet &a) {
    PacketPrinter::visit(static_cast<IP_Packet::super&>(a));
    a.ip_header().print();
}
