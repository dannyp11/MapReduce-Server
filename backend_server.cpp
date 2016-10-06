/*
 * backend_server.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Dat Pham
 */

#include "backend_server.h"

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

BackendServer::BackendServer(int local_port, int aws_port, string aws_address,
		string name) :
		Server(local_port, name), mAWSAddress(aws_address), mAWSPort(aws_port)
{

	mIsAws = false;

	bool initIsGood = initServer();
	if (!initIsGood)
	{
		LOG("Error initializing server " + name);
	}
}

BackendServer::~BackendServer()
{
	// TODO Auto-generated destructor stub
}

bool BackendServer::initServer()
{
	bool res = Server::initServer();

	// get server host name
	mAwsServer_hostent = gethostbyname(mAWSAddress.c_str());
	if (mAwsServer_hostent == NULL)
	{
		LOG("Error: can't connect to " + mAWSAddress);
		exit(1);
	}

	// setup IP for AWS
	mAwsSockaddr_in.sin_family = AF_INET;
	memcpy((char *) &mAwsSockaddr_in.sin_addr.s_addr,
			(char *) mAwsServer_hostent->h_addr, mAwsServer_hostent->h_length);
	mAwsSockaddr_in.sin_port = htons(mAWSPort);

	// let user knows it runs
	cout << "The Server " << mName << " is up and running using UDP on port "
			<< mUDPLocalPort << endl;

	return res;
}

void BackendServer::runServer()
{
	// setup recving message
	char buf[BUF_LEN];
	unsigned msg_len = sizeof(struct sockaddr_storage);

	while (recvfrom(mUDPLocalSockFd, buf, sizeof(buf), 0,
			(struct sockaddr*) &mAwsSockaddr_in, &msg_len) > 0)
	{
		// process received message
		// todo
		int len = strlen(buf);
		string buf_s(buf, len);
		TRACE(buf_s);

		break;
	}
}
