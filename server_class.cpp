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
#include <unistd.h>

#include <iostream>

using std::cout;
using std::endl;

Server::~Server()
{
	// TODO Auto-generated destructor stub
	close(mUDPLocalSockFd);
}

Server::Server(int remote_port, int local_port, string name) :
		mName(name), mUDPLocalPort(local_port)
{
	mUDPLocalSockFd = -1;

	if (name == "AWS")
	{
		mIsAws = true;
	}
	else
	{
		mIsAws = false;
	}

//	initServer();
}

bool Server::initServer()
{
	// Init UDP listening server

	bool res = true;

	// create listener socket
	mUDPLocalSockFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (mUDPLocalSockFd < 0)
	{
		perror("Error opening socket");
		exit(EXIT_FAILURE);
		res = false;
	}

//	/*
//	 * reuse port
//	 */
//	int optval = 1;
//	setsockopt(mUDPLocalSockFd, SOL_SOCKET, SO_REUSEPORT,
//			(const void *) &optval, sizeof(int));



	return res;
}

void Server::runServer()
{
}

long Server::getMin(const vector<long>& vec_data)
{
	long result = *vec_data.begin();

	for (vector<long>::const_iterator it = vec_data.begin();
			it != vec_data.end(); ++it)
	{
		if (result > *it)
		{
			result = *it;
		}
	}

	return result;
}

long Server::getMax(const vector<long>& vec_data)
{
	long result = *vec_data.begin();

	for (vector<long>::const_iterator it = vec_data.begin();
			it != vec_data.end(); ++it)
	{
		if (result < *it)
		{
			result = *it;
		}
	}

	return result;
}

long Server::getSum(const vector<long>& vec_data)
{
	long result = 0;

	for (vector<long>::const_iterator it = vec_data.begin();
			it != vec_data.end(); ++it)
	{
		result += *it;
	}

	return result;
}

long Server::getSos(const vector<long>& vec_data)
{
	long result = 0;

	for (vector<long>::const_iterator it = vec_data.begin();
			it != vec_data.end(); ++it)
	{
		result += (*it) * (*it);
	}

	return result;
}
