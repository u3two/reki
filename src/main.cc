#include <atomic>
#include <iostream>

#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <thread>

#include "packet_stream.hpp"
#include "visitors/visitors.hpp"
#include "gui/gui.hpp"

#include "appstate.hpp"

AppState APP_STATE;
std::atomic_bool quit = false;

int fetch_packets()
{
    auto stream = PacketStream::get();
    auto printer = PacketPrinter {};
    printer.hexdump = true;

    while (!quit) {
        auto optnxt = stream->next();
        if (optnxt) {
            std::unique_ptr<Packet> nxt = std::move(*optnxt);

            //nxt->apply(printer);
            //std::cout << std::endl;

            std::lock_guard<std::mutex> lck { APP_STATE.mutex };
            APP_STATE.packet_store.push_back(std::move(nxt));
        }
    }

    return 0;
}

int main() 
{
    gui::init();

    std::thread packet_fetcher(fetch_packets);

    gui::launch();
    quit = true;

    packet_fetcher.join();
    return 0;
}
