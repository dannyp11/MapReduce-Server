#include <iostream>
#include <string>

#include "common.h"
#include "server_class.h"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char** argv)
{
	parse_command_line_args(argc, argv);

	Server serverA("nunki.usc.edu", UDP_PORT_A, "A");

	cout << "Server A" << endl;

	return 0;
}
