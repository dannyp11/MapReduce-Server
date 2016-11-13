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
#include <string.h>

#include <iostream>
#include "common.h"

using std::cout;
using std::endl;

Server::~Server()
{
	// TODO Auto-generated destructor stub
	close(mUDPLocalSockFd);
}

Server::Server(int local_port, string name) :
		mName(name), mUDPLocalPort(local_port)
{
	mUDPLocalSockFd = -1;
	isInitSuccess = false;

	if (name == "AWS" || name == "aws")
	{
		mIsAws = true;
	}
	else
	{
		mIsAws = false;
	}
}

bool Server::initServer()
{
	// Init UDP listening server

	bool res = true;

	// create listener socket
	mUDPLocalSockFd = socket(AF_INET, SOCK_DGRAM, 0);
	if (mUDPLocalSockFd < 0)
	{
		res = false;
		perror("Error opening socket");
		exit(EXIT_FAILURE);
	}

	// setup local IP
	memset(&mUDPLocalAddr_in, '\0', sizeof(mUDPLocalAddr_in));
	mUDPLocalAddr_in.sin_family = AF_INET;
	mUDPLocalAddr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	mUDPLocalAddr_in.sin_port = htons(mUDPLocalPort);

	// bind local udp to socket
	if (bind(mUDPLocalSockFd, (struct sockaddr*) &mUDPLocalAddr_in,
			sizeof(mUDPLocalAddr_in)) < 0)
	{
		LOG("Error binding socket of server " + mName);
		res = false;
	}

	isInitSuccess = res;
	return res;
}

void Server::runServer()
{
	if (!isInitSuccess)
	{
		cout << "Can't run server because it fails initing" << endl;
		return;
	}
}

long Server::getMin(const vector<long>& vec_data) const
{
	long result = *vec_data.begin();

	if (vec_data.size() == 0)
	{
		return LONG_MAX;
	}

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

long Server::getMax(const vector<long>& vec_data) const
{
	long result = *vec_data.begin();

	if (vec_data.size() == 0)
	{
		return LONG_MIN;
	}

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

long Server::getSum(const vector<long>& vec_data) const
{
	long result = 0;

	for (vector<long>::const_iterator it = vec_data.begin();
			it != vec_data.end(); ++it)
	{
		result += *it;
	}

	return result;
}

long Server::getSos(const vector<long>& vec_data) const
{
	long result = 0;

	for (vector<long>::const_iterator it = vec_data.begin();
			it != vec_data.end(); ++it)
	{
		result += (*it) * (*it);
	}

	return result;
}

string Server::getCalcCommandName(CalcCommand cmd) const
{
	string result = "Invalid ClientCalcCommand";

	switch (cmd)
	{
	case SUM:
		result = "SUM";
		break;
	case MIN:
		result = "MIN";
		break;
	case MAX:
		result = "MAX";
		break;
	case SOS:
		result = "SOS";
		break;
	case RESULT:
		result = "RESULT";
		break;
	}

	return result;
}

vector<long> Server::getVectorFromArray(int data_len, const long data[]) const
{
	vector<long> result;

	for (int i = 0; i < data_len; ++i)
	{
		result.push_back(data[i]);
	}

	return result;
}
