#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <thread>
#include <unistd.h>

#include "packet_stream.hpp"
#include "gui/gui.hpp"

#include "appstate.hpp"

AppState APP_STATE;

int fetch_packets(int cancelfd)
{
    auto stream = PacketStream::create(cancelfd);

    while (auto optnxt = stream->next()) {
        std::lock_guard<std::mutex> lck { APP_STATE.mutex };
        APP_STATE.packet_store.push_back(std::move(*optnxt));
    }

    return 0;
}

int main() 
{
    gui::init();

    /// TODO: wrap in some class?
    int cancelfds[2];
    pipe(cancelfds);

    std::thread packet_fetcher(fetch_packets, cancelfds[0]);

    gui::launch();

    write(cancelfds[1], "#", 1);
    packet_fetcher.join();
    return 0;
}
