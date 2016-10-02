#include <iostream>
#include <string>

#include "common.h"

using std::cout;
using std::endl;
using std::string;


int main(int argc, char** argv) {
	parse_command_line_args(argc, argv);

	cout << "Server A" << endl;

	return 0;
}
