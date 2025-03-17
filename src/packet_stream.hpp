#ifndef REKI_PACKET_STREAM
#define REKI_PACKET_STREAM

#include "packet.hpp"

#include <memory>

class PacketStream {
public:
    virtual std::unique_ptr<Packet> next() = 0;
    virtual ~PacketStream() {};

    /// Platform dependent stream creation
    static std::unique_ptr<PacketStream> get();
};

class LinuxPacketStream final : public PacketStream {
private:
    int m_sockfd;
    /// Fetch raw data of the next packet
    std::vector<u8> fetch_next();
public:
    LinuxPacketStream();
    ~LinuxPacketStream();
    std::unique_ptr<Packet> next() override;
};

class WindowsPacketStream final : public PacketStream {
public:
    std::unique_ptr<Packet> next() override;
};

#endif
