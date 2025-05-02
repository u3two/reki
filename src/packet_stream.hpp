#ifndef REKI_PACKET_STREAM
#define REKI_PACKET_STREAM

#include "packet.hpp"

#include <memory>
#include <optional>
#include <sys/epoll.h>

class PacketStream {
public:
    virtual std::optional<std::unique_ptr<Packet>> next() = 0;
    virtual ~PacketStream() {};

    /// Platform dependent stream creation
    static std::unique_ptr<PacketStream> create(int cancelfd);
};

class LinuxPacketStream final : public PacketStream {
private:
    static constexpr auto MAX_EVENTS = 2;
    epoll_event m_events[MAX_EVENTS];
    int m_sockfd, m_epollfd, m_cancelfd;

    /// (blocking) fetch of the next packet's raw data
    std::vector<u8> fetch_next();
public:
    explicit LinuxPacketStream(int cancelfd);
    ~LinuxPacketStream() override;
    std::optional<std::unique_ptr<Packet>> next() override;
};

#endif
