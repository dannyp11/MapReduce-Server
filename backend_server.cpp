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
		exit(EXIT_FAILURE);
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
	Server::runServer();

	// setup recving message
	char buf[sizeof(ServerMessage)];
	socklen_t msg_len = sizeof(struct sockaddr_storage);

	while (recvfrom(mUDPLocalSockFd, buf, sizeof(buf), 0,
			(struct sockaddr*) &mAwsSockaddr_in, &msg_len) > 0)
	{
		// process received message
		memcpy(&mMessage, buf, sizeof(buf));

		vector<long> messageData = getVectorFromArray(mMessage.entriesCount, mMessage.data);

		// output to console
		cout << "The Server " << mName << " has received "
				<< mMessage.entriesCount << " numbers" << endl;

		// determine the sender
		if (mAwsSockaddr_in.sin_port != htons(UDP_PORT_AWS))
		{
			cout << "You're not AWS! Your port is " << mAwsSockaddr_in.sin_port
					<< endl;
			continue;
		}

		// calculate
		switch (mMessage.command)
		{
		case SUM:
		{
			mMessage.resultValue = getSum(messageData);
			cout << "The Server " << mName << " has successfully finished the reduction SUM: "
					<< mMessage.resultValue << endl;
		}
			break;

		case MIN:
		{
			mMessage.resultValue = getMin(messageData);
			cout << "The Server " << mName << " has successfully finished the reduction MIN: "
								<< mMessage.resultValue << endl;
		}
			break;

		case MAX:
		{
			mMessage.resultValue = getMax(messageData);
			cout << "The Server " << mName << " has successfully finished the reduction MAX: "
								<< mMessage.resultValue << endl;
		}
			break;

		case SOS:
		{
			mMessage.resultValue = getSos(messageData);
			cout << "The Server " << mName << " has successfully finished the reduction SOS: "
								<< mMessage.resultValue << endl;
		}
			break;

		default:
			cout << "Invalid command" << endl;
			break;
		}

		mMessage.command = RESULT;
		mMessage.entriesCount = 0;
		strncpy(mMessage.serverName, mName.c_str(), mName.length());
		mMessage.serverName[mName.length()] = '\0';

		// send back to sender
		memcpy(buf, &mMessage, sizeof(mMessage));
		int byte_sent = sendto(mUDPLocalSockFd, buf, sizeof(buf), 0,
				(sockaddr*) &mAwsSockaddr_in, sizeof(mAwsSockaddr_in));

		if (byte_sent < 0)
		{
			perror("Can't send result to AWS");
		}
		else
		{
			cout << "The Server " << mName << " has successfully finished sending"
					<< " the reduction value to AWS server" << endl;
		}

		// clear buf
		memset(&buf, 0, sizeof(buf));
	}
}
