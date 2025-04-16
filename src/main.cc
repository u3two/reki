#include "packet_capture.hpp"
#include "gui/gui.hpp"

#include "appstate.hpp"

AppState APP_STATE;

int main() 
{
    gui::init();
    gui::launch();

    APP_STATE.capture.stop();
    return 0;
}
