
#include <iostream>
#include "dicerandom.h"
#include "dicesequence.h"
#include "mud.h"
#include "mudserver.h"
int main(int argc, char *argv[]) {
	MudServer server;
	server.start();
	Mud mud;
	mud.start();

	mud.stop();
	server.stop();
}
