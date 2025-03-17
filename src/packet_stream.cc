// TODO: separate into two files? (linux_packet_stream.cc, etc.)
#include "packet_stream.hpp"
#include "defs.hpp"

#include "protocols/ethernet.hpp"

#include <memory>

#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <arpa/inet.h>


std::unique_ptr<PacketStream> PacketStream::get()
{
#ifdef __linux__
    return std::make_unique<LinuxPacketStream>();
#endif

#ifdef _WIN32
    return std::make_unique<WindowsPacketStream>();
#endif
}

LinuxPacketStream::LinuxPacketStream() 
{
    // NOTE: bind can be used to bind to a specific interface,
    //       this listens to all of them (?)
    this->m_sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (this->m_sockfd == -1) {
        perror("opening socket");
        throw std::runtime_error("failed to create AF_PACKET socket");
    }
}

LinuxPacketStream::~LinuxPacketStream() 
{
    close(this->m_sockfd);
}

std::vector<u8> LinuxPacketStream::fetch_next() 
{
    std::vector<u8> buff;
    buff.resize(8192);

    ssize_t recvd = recv(this->m_sockfd, buff.data(), buff.capacity(), 0);
    if (recvd == -1) {
        perror("receiving from sock");
        throw std::runtime_error("failed to create recv() data");
    }

    buff.resize(recvd);
    return buff;
}

std::unique_ptr<Packet> LinuxPacketStream::next() {
    auto data = this->fetch_next();

    // TODO: this is mostly temporary? we should make packet construction super cheap
    // and then just construct ethernet -> check ethertype -> ip (or whatever) -> tcp (...)
    // determine packet type, for now we assume that:
    //  - all packets we receive contain valid ethernet frames
    // const EthernetHeader *ehdr = reinterpret_cast<EthernetHeader*>(data.data());
    // switch (ntohs(ehdr->ethertype)) {
    //     case static_cast<int>(EtherType::IPv4): {
    //         const IP_Header *iphdr = reinterpret_cast<IP_Header*>(data.data() + sizeof(EthernetHeader));
    //         switch (iphdr->protocol) {
    //             case static_cast<int>(IP_Protocol::TCP): {
    //                 return std::make_unique<TCP_Packet>(data);
    //             } break;
    //         }
    //         return std::make_unique<IP_Packet>(data);
    //     } break;
    // }

    // Assume for now that all packets coming our way are ethernet packets
    auto eth = std::make_unique<EthernetPacket>(data);

    // unhandled type, just construct the base class packet
    return std::make_unique<EthernetPacket>(data);
}
