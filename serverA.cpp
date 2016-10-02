#include <iostream>
#include <string>

#include "backend_server.h"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char** argv)
{
	parse_command_line_args(argc, argv);

	BackendServer serverA(UDP_PORT_A, UDP_PORT_AWS, "localhost", "A");

	serverA.runServer();

	return 0;
}
