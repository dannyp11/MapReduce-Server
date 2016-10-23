/*
 * aws_server.cpp
 *
 *  Created on: Oct 22, 2016
 *      Author: dat
 */

#include "aws_server.h"

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

#include <iostream>

using std::cout;
using std::endl;

AWSServer::AWSServer() :
		Server(UDP_PORT_AWS, "AWS")
{
	// TODO Auto-generated constructor stub
	bool initIsGood = initServer();
	if (!initIsGood)
	{
		LOG("Error initializing server " + mName);
	}

}

AWSServer::~AWSServer()
{
	// TODO Auto-generated destructor stub
}

bool AWSServer::initServer()
{
	bool result = Server::initServer();

	// let user knows it runs
	cout << "The " << mName << " is up and running" << endl;

	return result;
}

void AWSServer::runServer()
{
}
