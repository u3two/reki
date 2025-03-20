#include <iostream>

#include "packet_stream.hpp"
#include "visitors/visitor.hpp"

int main()
{
    auto stream = PacketStream::get();

    auto printer = PacketPrinter {};
    printer.hexdump = true;

    for (;;) {
        auto nxt = stream->next();
        nxt->apply(printer);
        std::cout << std::endl;
    }
}
