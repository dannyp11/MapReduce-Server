#include "common.h"

#include <iostream>
#include <sys/types.h>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

using std::cout;
using std::endl;
using std::string;

void show_usage(const char* bin_name)
{
	cout << "usage: " << bin_name << " [-OPTION] [VALUE]" << endl;
	cout << "		[-n server name (backend only)]" << endl;
	cout << "		[-i show binary info]" << endl;
	cout << "		[-h show help]" << endl;
	exit(1);
}

void parse_command_line_args(int argc, char** argv, string& server_name)
{
	char* exec_name = *argv;
	signed char command;

	while ((command = getopt(argc, argv, "ihn:")) != -1)
	{
		switch (command)
		{
		case 'n':
		{
			server_name = optarg;
			break;
		}

		case 'i':
		{
			cout << "Built: " << __DATE__ << " " << __TIME__ << endl;

			string cpp_version = "";
			if ( __cplusplus == 201103L)
				cpp_version = "C++11";
			else if ( __cplusplus == 199711L)
				cpp_version = "C++98";
			else
				cpp_version = "pre-standard C++";

			cout << "Compiler: " << cpp_version << " version " << __VERSION__
					<< endl;
			exit(0);
		}
			break;

		case 'h':
			show_usage(exec_name);
			break;

		default:
//			show_usage(exec_name);
			break;
		}

		if (optind < argc)
		{
			//			show_usage(exec_name);
		}
	}
}

string getClientCalcCommandName(ClientCalcCommand cmd)
{
	string result = "Invalid ClientCalcCommand";

	switch (cmd)
	{
	case CSUM:
		result = "SUM";
		break;
	case CMIN:
		result = "MIN";
		break;
	case CMAX:
		result = "MAX";
		break;
	case CSOS:
		result = "SOS";
		break;
	case CRESULT:
		result = "RESULT";
		break;
	}

	return result;
}
