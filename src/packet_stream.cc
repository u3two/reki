// TODO: separate into two files? (linux_packet_stream.cc, etc.)
#include "packet_stream.hpp"
#include "defs.hpp"

#include "protocols/ethernet.hpp"
#include "protocols/ip.hpp"
#include "protocols/tcp.hpp"
#include "protocols/udp.hpp"
#include "protocols/arp.hpp"

#include <memory>

#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <arpa/inet.h>

#include <net/ethernet.h>


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

    // assume for now that all packets coming our way are ethernet packets.
    auto eth = EthernetPacket { std::move(data) };

    // this whole "upgrade" process could probably be streamlined into a .upgrade() thing
    // for every type, returning maybe something like a pair<Packet, bool> to indicate
    // whether an upgrade was successful
    switch (eth.eth_header().data().ethertype) {
        case static_cast<u16>(EtherType::IPv4): {
            auto ip = IP_Packet { std::move(eth) };
            switch (ip.ip_header().data().protocol) {
                case static_cast<u16>(IP_Protocol::TCP): {
                    return std::make_unique<TCP_Packet>(std::move(ip));
                } break;
                case static_cast<u16>(IP_Protocol::UDP): {
                    return std::make_unique<UDP_Packet>(std::move(ip));
                } break;
                case static_cast<u16>(IP_Protocol::ICMP): {
                    // TODO: ICMP
                } break;
            }
            // unhandled ip protocol, return as ip packet
            return std::make_unique<IP_Packet>(std::move(ip));
        } break;
        case static_cast<u16>(EtherType::ARP): {
            return std::make_unique<ARP_Packet>(std::move(eth));
        };
    }
    // unhandled ethertype, return as ethernet packet
    return std::make_unique<EthernetPacket>(std::move(eth));
}
