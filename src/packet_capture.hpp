#ifndef REKI_PACKET_CAPTURE
#define REKI_PACKET_CAPTURE

#include <optional>
#include <thread>

class PacketCapture {
private:
    int m_cancelfds[2];
    std::optional<std::thread> m_thread;
public:
    PacketCapture();
    ~PacketCapture();

    /// begin capturing packets.
    /// spawns a new thread and returns immediately.
    void start();

    /// stop capturing packets.
    /// kills the packet capturing thread.
    /// packet capture may be resumed by calling .start() again.
    void stop();
};

#endif /* REKI_PACKET_CAPTURE */
