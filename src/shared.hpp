#ifndef REKI_SHARED_H
#define REKI_SHARED_H

#include "packet.hpp"

#include <mutex>
#include <memory>

extern std::vector<std::unique_ptr<Packet>> PACKET_STORE;
extern std::mutex PACKET_STORE_MUTEX;

enum class AppMode {
    Capture,
    Browse
};

extern AppMode APPMODE;

#endif /* REKI_SHARED_H */
