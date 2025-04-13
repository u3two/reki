#include "packet_capture.hpp"
#include "packet_stream.hpp"
#include "appstate.hpp"

#include <optional>
#include <unistd.h>

#include <mutex>

PacketCapture::PacketCapture()
: m_thread{}
{
    pipe(m_cancelfds);
}

PacketCapture::~PacketCapture() {
    close(m_cancelfds[0]);
    close(m_cancelfds[1]);
}

void PacketCapture::start()
{
    if (m_thread.has_value())
        return; // don't spawn extra threads (maybe this should throw?)

    m_thread = std::thread([this] {
        auto stream = PacketStream::create(m_cancelfds[0]);

        while (auto optnxt = stream->next()) {
            std::lock_guard<std::mutex> lck { APP_STATE.mutex };
            APP_STATE.packet_store.push_back(std::move(*optnxt));
        }

        return 0;
    });
}

void PacketCapture::stop()
{
    if (!m_thread.has_value())
        return; // don't stop nonexistent threads (maybe this should throw?)

    write(m_cancelfds[1], "#", 1);
    m_thread->join();

    m_thread = std::nullopt;
}
