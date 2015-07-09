
#include <iostream>
#include "dicerandom.h"
#include "dicesequence.h"
#include "mud.h"
#include "mudserver.h"
#include "textgen/color.h"
#include <boost/locale.hpp>

int main(int argc, char *argv[]) {
    boost::locale::generator gen;
    std::locale::global(gen(""));

    text::Color::loadColors();
    MudServer server;
    server.start();
    Mud mud;
    mud.start();
    server.stop();
}
