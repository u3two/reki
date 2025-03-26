#include <iostream>

#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>

#include "packet_stream.hpp"
#include "visitors/visitors.hpp"

#include "gui/gui.hpp"

int main() 
{
    gui_init();

    return gui_main();
}

int main3() 
{
    const char *file = "../db/packets.sqlite3";

    try {
        soci::session sql(soci::sqlite3, file);
    } catch (soci::soci_error const& e) {
        std::cerr << "Connection to \"" << file << "\" failed: "
                  << e.what() << "\n";
    }
    
    return 0;
}

int main2()
{
    auto stream = PacketStream::get();

    auto printer = PacketPrinter {};
    printer.hexdump = true;

    for (;;) {
        auto nxt = stream->next();
        nxt->apply(printer);
        std::cout << std::endl;
    }

    return 0;
}
