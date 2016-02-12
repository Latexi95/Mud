
#include <iostream>
#include "dicerandom.h"
#include "dicesequence.h"
#include "mud.h"
#include "mudserver.h"
#include "textgen/color.h"
#include <boost/locale.hpp>

static Mud mud;


//Handle ctrl + c
#ifdef WIN32

#include "windows.h"

BOOL signalHandler( DWORD ctrlType)
{
    mud.shutdown();
    return TRUE;
}

void setupSignalHandler()
{
    SetConsoleCtrlHandler( (PHANDLER_ROUTINE)&signalHandler, TRUE);
}


#else
#include <csignal>
#include <cstdlib>
#include <unistd.h>

void signalHandler(int s)
{
    mud.shutdown();
}

void setupSignalHandler()
{
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = &signalHandler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
}

#endif



int main(int argc, char *argv[])
{

    boost::locale::generator gen;
    std::locale::global(gen(""));

    if (!mud.loadResources()) {
        std::cerr << "Failed to load all basic resources" << std::endl;
        return 0;
    }

    setupSignalHandler();

    text::Color::loadColors();
    MudServer server;
    server.start();
    mud.start();
    server.stop();

    std::cout << "Bye bye..." << std::endl;
}
