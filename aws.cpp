#include <iostream>
#include <string>

#include "aws_server.h"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char** argv)
{
	parse_command_line_args(argc, argv);

	AWSServer server;
	server.runServer();

	return 0;
}
