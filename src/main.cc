#include "packet_capture.hpp"
#include "gui/gui.hpp"

#include <sys/capability.h>

#include "appstate.hpp"

AppState APP_STATE;

// verify that the process was run with the CAP_NET_RAW Linux capability enabled, 
// so that it can open raw PACKET sockets.
void check_capabilities()
{
    cap_t current = cap_get_proc();
    cap_flag_value_t flag;
    cap_get_flag(current, CAP_NET_RAW, CAP_PERMITTED, &flag);
    if (!flag) {
        std::cerr << "The CAP_NET_RAW capability is missing from this executable!\n"
                     "Please run reki as root or add the missing capability.\n";
        std::exit(1);
    }
}

int main() 
{
    check_capabilities();

    gui::init();
    gui::launch();

    APP_STATE.capture.stop();
    return 0;
}
