#include <iostream>

#include "packet_stream.hpp"

int main()
{
    auto stream = PacketStream::get();
    for (;;) {
        auto nxt = stream->next();
        nxt->print_data();
        nxt->print();
        std::cout << std::endl;
    }
}
