#include <atomic>

#include <cstring>
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <thread>
#include <iostream>

#include "packet_stream.hpp"
#include "visitors/visitors.hpp"
#include "shared.hpp"

#include "gui/gui.hpp"

std::vector<std::unique_ptr<Packet>> PACKET_STORE;
std::mutex PACKET_STORE_MUTEX;
AppMode APPMODE;

std::atomic_bool quit = false;

int capture_packets()
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

void fetch_packets(const char *db_string)
{

}

void usage(const char *progname)
{
    std::cerr << "usage: " << progname << " [browse [DB_STRING]]\n";
}

int main(int argc, char **argv) 
{
    if (argc == 1) {
        // no arguments supplied, set defaults
        #ifdef _WIN32
            std::cerr << "Running under Windows; the default capture mode is unavailable. "
                         "Please use the browse mode (" << argv[0] << " browse [DB_STRING])\n";
            std::exit(1);
        #endif

        APPMODE = AppMode::Capture;
    } else {
        if (!strcmp(argv[1], "browse")) {
            if (!argv[2]) {
                std::cerr << "No DB connection string supplied!\n";
                usage(argv[0]);
                std::exit(1);
            }
            APPMODE = AppMode::Browse;
        } else if (!strcmp(argv[1], "help")) {
            usage(argv[0]);
            std::exit(0);
        } else {
            std::cerr << "Unknown option: '" << argv[1] << "'\n";
            usage(argv[0]);
            std::exit(1);
        }
    }

    gui_init();

    switch (APPMODE) {
        case AppMode::Capture: {
            std::thread packet_fetcher(capture_packets);

            gui_main();
            quit = true;

            packet_fetcher.join();
        } break;
        case AppMode::Browse: {
            fetch_packets(argv[2]);
            gui_main();
        } break;
    }

    return 0;
}
