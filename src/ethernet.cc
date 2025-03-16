#include "ethernet.hpp"

#include <iomanip>
#include <iostream>

#include <arpa/inet.h>

void EthernetHeader::print() const {
    std::cout << "[Ethernet Header]\n";

    std::cout << std::uppercase << std::hex 
              << std::setw(2) << std::setfill('0');

    std::cout << "Destination: ";
    for (int i = 0; i < destination_size; i++)
        std::cout << +this->destination[i]
                  << ((i != destination_size - 1) ? ":" : "");
    std::cout << std::endl;

    std::cout << "Source: ";
    for (int i = 0; i < destination_size; i++)
        std::cout << +this->source[i]
                  << ((i != destination_size - 1) ? ":" : "");
    std::cout << std::endl;

    u16 as_host = ntohs(this->ethertype);
    std::cout << "EtherType: ";
    if (as_host <= 1500) {
        std::cout << std::dec << "size(" << as_host << ")\n";
    } else if (as_host <= 1535) {
        std::cout << "UNDEFINED (malformed packet?)";
    } else {
        std::cout << "0x" << as_host;
        switch (as_host) {
            case static_cast<u16>(EtherType::IPv4): 
                std::cout << " (IPv4)";
                break;
            case static_cast<u16>(EtherType::IPv6): 
                std::cout << " (IPv6)";
                break;
            case static_cast<u16>(EtherType::WakeOnLan): // ether-wake
                std::cout << " (Wake-on-Lan)";
                break;
        }
        std::cout << std::endl;
    }

    std::cout << std::dec;
}
