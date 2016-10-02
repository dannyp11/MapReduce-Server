/*
 * server_class.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Dat
 */

#include "server_class.h"

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>

#include <iostream>

using std::cout;
using std::endl;

Server::~Server()
{
	// TODO Auto-generated destructor stub
}

Server::Server(string address, int port_number, string name) :
		mName(name), mAddress(address), mPort(port_number)
{
	if (name == "AWS")
	{
		mIsAws = true;
		initServerAWS();
	}
	else
	{
		mIsAws = false;
		initServer();
	}
}

void Server::initServer()
{
	// create socket
	mSockFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (mSockFd < 0)
	{
		perror("Error opening socket");
		exit (EXIT_FAILURE);
	}

//	// convert server name
//	struct hostent* server;
//	server = gethostbyname(mAddress.c_str()); // default server
//	if (server == NULL)
//	{
//		std::string msg("Can't connect to ");
//		msg.append(mAddress);
//		perror(msg.c_str());
//	}
}

void Server::initServerAWS()
{
}
