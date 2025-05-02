// TODO: separate into two files? (linux_packet_stream.cc, etc.)
#include "packet_stream.hpp"
#include "defs.hpp"

#include "protocols/ethernet.hpp"
#include "protocols/icmp.hpp"
#include "protocols/ip.hpp"
#include "protocols/tcp.hpp"
#include "protocols/udp.hpp"
#include "protocols/arp.hpp"

#include <cerrno>
#include <memory>

#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <arpa/inet.h>

#include <net/ethernet.h>


std::unique_ptr<PacketStream> PacketStream::create(int cancelfd)
{
#ifdef __linux__
    return std::make_unique<LinuxPacketStream>(cancelfd);
#endif

#ifdef _WIN32
    return std::make_unique<WindowsPacketStream>();
#endif
}

LinuxPacketStream::LinuxPacketStream(int cancelfd) 
: m_cancelfd(cancelfd)
{
    // NOTE: bind can be used to bind to a specific interface,
    //       this listens to all of them (?)
    this->m_sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (this->m_sockfd == -1) {
        perror("opening socket");
        throw std::runtime_error("failed to create AF_PACKET socket");
    }

    m_epollfd = epoll_create1(0);
    if (m_epollfd == -1)
        throw std::runtime_error("failed to epoll_create1");

    epoll_event ev;

    // add our base network socket
    ev.events = EPOLLIN;
    ev.data.fd = m_sockfd;
    if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, m_sockfd, &ev) == -1)
        throw std::runtime_error("failed to epoll_ctl ADD (net)");

    // add the cancel signaling socket
    ev.data.fd = cancelfd;
    if (epoll_ctl(m_epollfd, EPOLL_CTL_ADD, cancelfd, &ev) == -1)
        throw std::runtime_error("failed to epoll_ctl ADD (cancel)");
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
        throw std::runtime_error("recv() failure");
    }

    buff.resize(recvd);
    return buff;
}

std::unique_ptr<Packet> packet_from_data(std::vector<u8> data)
{
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
                    return std::make_unique<ICMP_Packet>(std::move(ip));
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

std::optional<std::unique_ptr<Packet>> LinuxPacketStream::next() {
    int nfds = epoll_wait(m_epollfd, m_events, MAX_EVENTS, -1);
    if (nfds == -1)
        throw std::runtime_error("failed to epoll_wait");

    for (int i = 0; i < nfds; i++) {
        if (m_events[i].data.fd == m_sockfd) {
            return packet_from_data(this->fetch_next());
        } else if (m_events[i].data.fd == m_cancelfd) {
            char b;
            read(m_cancelfd, &b, 1); // discard the signaling char
            return {};
        }
    }

    throw std::runtime_error("unexpected epoll_wait return");
}
