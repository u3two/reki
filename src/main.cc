#include "packet_capture.hpp"
#include "gui/gui.hpp"

#include "appstate.hpp"

AppState APP_STATE;

int main() 
{
    PacketCapture capture {};
    gui::init();

    capture.start();

    gui::launch();

    capture.stop();
    return 0;
}
