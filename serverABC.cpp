#include <iostream>
#include <string>
#include <cstdlib>
#include "backend_server.h"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char** argv)
{
	string serverName = "A";
	parse_command_line_args(argc, argv, serverName);

	int port;
	if (serverName == "A")
	{
		port = UDP_PORT_A;
	}
	else if (serverName == "B")
	{
		port = UDP_PORT_B;
	}
	else if (serverName == "C")
	{
		port = UDP_PORT_C;
	}
	else
	{
		cout << "Error: invalid server name, only A, B, or C allowed" << endl;
		exit(1);
	}

	BackendServer backend(port, UDP_PORT_AWS, "localhost", serverName);
	backend.runServer();

	return 0;
}
