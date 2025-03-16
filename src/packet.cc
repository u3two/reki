#include "packet.hpp"
#include "ethernet.hpp"

#include <iomanip>
#include <iostream>

#include <arpa/inet.h>

void Packet::print() const {
    std::cout << "[Packet] " << this->length() << " Bytes\n";
    this->m_etherhdr->print();
}

void Packet::print_data() const
{
    std::cout << std::uppercase << std::hex;

    constexpr size_t cols = 20;
    size_t counter = 0;
    for (u8 c : this->m_data) {
        std::cout << std::setw(2) << +c << " ";
        counter++;
        if (counter == cols) {
            std::cout << std::endl;
            counter = 0;
        } 
    }
    std::cout << std::endl;

    std::cout << std::dec;
}

Packet::Packet(std::vector<u8> data)
: m_data(data)
{
    if (data.size() < sizeof(EthernetHeader)) 
        throw std::runtime_error("Packet data size too small!");

    this->m_etherhdr = reinterpret_cast<const EthernetHeader*>(this->m_data.data());
    this->m_offset = sizeof(EthernetHeader);
}
