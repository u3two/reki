// TODO: separate into two files? (linux_packet_stream.cc, etc.)
#include "packet_stream.hpp"
#include "defs.hpp"

#include "ip_packet.hpp"

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

    // TODO: resolve packet type
    return std::make_unique<IP_Packet>(data);
}
