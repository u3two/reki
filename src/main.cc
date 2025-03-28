#include <atomic>
#include <iostream>

#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <thread>

#include "packet_stream.hpp"
#include "visitors/visitors.hpp"
#include "shared.hpp"

#include "gui/gui.hpp"

std::vector<std::unique_ptr<Packet>> PACKET_STORE;
std::mutex PACKET_STORE_MUTEX;

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

            std::lock_guard<std::mutex> lck { PACKET_STORE_MUTEX };
            PACKET_STORE.push_back(std::move(nxt));
        }
    }

    return 0;
}

int main() 
{
    gui_init();

    std::thread packet_fetcher(fetch_packets);

    gui_main();
    quit = true;

    packet_fetcher.join();

    return 0;
}
