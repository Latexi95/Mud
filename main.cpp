
#include <iostream>
#include "dicerandom.h"
#include "dicesequence.h"
#include "mud.h"
#include "mudserver.h"
#include "textgen/color.h"
int main(int argc, char *argv[]) {
	text::Color::loadColors();
	MudServer server;
	server.start();
	Mud mud;
	mud.start();

	mud.stop();
	server.stop();
}
