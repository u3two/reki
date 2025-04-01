#ifndef REKI_STATE_H
#define REKI_STATE_H

#include "packet.hpp"

#include <mutex>
#include <memory>

struct AppState {
    /// Contains all the packets captured in the current session.
    std::vector<std::unique_ptr<Packet>> packet_store;

    

    /// Global appstate mutex; needs to be taken in order to safely access
    /// any of the data stored in this structure.
    std::mutex mutex;
};

extern AppState APP_STATE;

#endif /* REKI_STATE_H */
