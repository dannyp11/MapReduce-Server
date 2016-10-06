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


	// debug stuff
	ServerMessage message;
	message.command = SUM;
	message.resultValue = 123456;
	message.serverName = "aws";

	for (int i =0; i< 1000;i++)
	{
		message.data.push_back(-i);
	}

//	TRACE(sizeof(message));

	return 0;
}
