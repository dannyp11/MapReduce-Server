#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string.h>

#include "common.h"

#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::vector;

const string input_file_path = "nums.csv";

/*
 * Internal variables
 */
static ClientMessage clientMessage;
static ClientMessage serverMessage;

int main(int argc, char** argv)
{
	// get data from input file -----------------------------------
	vector<long> vec_input_data;
	std::ifstream input_file;
	input_file.open(input_file_path.c_str());
	if (!input_file.is_open())
	{
		cout << "Error opening file " << input_file_path << endl;
		exit(EXIT_FAILURE);
	}

	long data;
	while (input_file >> data)
	{
		vec_input_data.push_back(data);
	}

	input_file.close();
	// get data from input file end--------------------------------

	//debug
//	vec_input_data.clear();
//	for (int i =0; i < 10; ++i)
//	{
//		vec_input_data.push_back(1000000);
//		TRACE(vec_input_data.at(i));
//	}
//	std::sort(vec_input_data.begin(), vec_input_data.end());
//	for (unsigned i = 0; i < vec_input_data.size();++i)
//	{
//		cout << vec_input_data.at(i) << endl;
//	}
//	return 0;

	// build clientMessage ----------------------------------------
	clientMessage.entriesCount = static_cast<int>(vec_input_data.size());

	if (clientMessage.entriesCount == 0)
	{
		cout << "Warning: empty input data" << endl;
		cout << "Client didn't send to AWS server" << endl;
		return 1;
	}

	// error checking
	if (argc != 2)
	{
		cout << "Invalid argument count" << endl;
		cout << "usage: " << argv[0] << " cmd" << endl;
		cout << "\t\t cmd could be sum, min, max, sos" << endl;
		exit(EXIT_FAILURE);
	}

	string input_cmd = argv[1];
	if (input_cmd == "sum")
	{
		clientMessage.command = CSUM;
	}
	else if (input_cmd == "min")
	{
		clientMessage.command = CMIN;
	}
	else if (input_cmd == "max")
	{
		clientMessage.command = CMAX;
	}
	else if (input_cmd == "sos")
	{
		clientMessage.command = CSOS;
	}
	else
	{
		cout << "Invalid command" << endl;
		cout << "usage: " << argv[0] << " cmd" << endl;
		cout << "\t\t cmd could be sum, min, max, sos" << endl;
		exit(EXIT_FAILURE);
	}

	//data writing
	for (int i =0; i< clientMessage.entriesCount; ++i)
	{
		clientMessage.data[i] = vec_input_data.at(i);
	}

	clientMessage.resultValue = LLONG_MAX;
	// build clientMessage end ------------------------------------

	// TCP data ----------------------------------------------------
	int localTCPSockfd;
	struct sockaddr_in awsTCPServer_sockaddr_in;
	struct hostent* awsServerhostent;

	// establish TCP connection -------------------------------------
	localTCPSockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (localTCPSockfd < 0)
	{
		perror("Error openning client socket");
		exit(EXIT_FAILURE);
	}

	awsServerhostent = gethostbyname("localhost");
	if (awsServerhostent == NULL)
	{
		cout << "Can't find to AWS server on network" << endl;
		exit(EXIT_FAILURE);
	}

	// define addr_in of aws
	awsTCPServer_sockaddr_in.sin_family = AF_INET;
	memcpy((char*) &awsTCPServer_sockaddr_in.sin_addr.s_addr,
			(char*) awsServerhostent->h_addr,
			awsServerhostent->h_length);
	awsTCPServer_sockaddr_in.sin_port = htons(TCP_PORT_AWS);

	if (connect(localTCPSockfd, (struct sockaddr*) &awsTCPServer_sockaddr_in,
			sizeof(awsTCPServer_sockaddr_in)) != 0)
	{
		perror("Error connecting TCP with aws");
		exit(EXIT_FAILURE);
	}

	cout << "The client is up and running." << endl;
	// establish TCP connection end -----------------------------------

	// send message ---------------------------------------------------
	char buf[sizeof(ClientMessage)];
	memcpy(buf, &clientMessage, sizeof(clientMessage));
	ssize_t sent_bytes = write(localTCPSockfd, buf, sizeof(buf));
	if (sent_bytes <= 0)
	{
		cout << "Error sending message to server" << endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		cout << "The client has sent the reduction type "
				<< getClientCalcCommandName(clientMessage.command)
				<< " to AWS" << endl;
		cout << "The client has sent " << clientMessage.entriesCount
				<< " numbers to AWS" << endl;
	}

	// receive message -------------------------------------------------
	ssize_t recv_bytes = read(localTCPSockfd, buf, sizeof(buf));
	if (recv_bytes <= 0)
	{
		cout << "Error receiving result message from server" << endl;
	}

	memcpy(&serverMessage, buf, sizeof(buf));
	if (serverMessage.command == CRESULT)
	{
		cout << "The client has received reduction "
				<< getClientCalcCommandName(clientMessage.command)
				<< ": " << serverMessage.resultValue << endl;
	}
	else
	{
		cout << "Client receives invalid message" << endl;
	}

	//clean up ---------------------------------------------------------
	close(localTCPSockfd);
	return 0;
}
