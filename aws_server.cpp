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
	// configure server A B C port here
	mServerAPort = UDP_PORT_A;
	mServerBPort = UDP_PORT_B;
	mServerCPort = UDP_PORT_C;

	// configure server A B C address here
	mServerAAddress = "localhost";
	mServerBAddress = "localhost";
	mServerCAddress = "localhost";

	bool initIsGood = initServer();
	if (!initIsGood)
	{
		LOG("Error initializing server " + mName);
	}
}

AWSServer::~AWSServer()
{
}

bool AWSServer::initServer()
{
	bool result = Server::initServer();

	// setup server A B C sockets -------------------------------
	mServerA = gethostbyname(mServerAAddress.c_str());
	mServerB = gethostbyname(mServerBAddress.c_str());
	mServerC = gethostbyname(mServerCAddress.c_str());

	if (mServerA == NULL || mServerB == NULL || mServerC == NULL)
	{
		perror("Can't connect to one of the server A, B, C");
		result = false;
	}

	// build sockaddr_in
	mASockaddr_in.sin_family = AF_INET;
	memcpy((char *) &mASockaddr_in.sin_addr.s_addr, (char *) mServerA->h_addr, mServerA->h_length);
	mASockaddr_in.sin_port = htons(mServerAPort);

	mBSockaddr_in.sin_family = AF_INET;
	memcpy((char *) &mBSockaddr_in.sin_addr.s_addr, (char *) mServerB->h_addr, mServerB->h_length);
	mBSockaddr_in.sin_port = htons(mServerBPort);

	mCSockaddr_in.sin_family = AF_INET;
	memcpy((char *) &mCSockaddr_in.sin_addr.s_addr, (char *) mServerC->h_addr, mServerC->h_length);
	mCSockaddr_in.sin_port = htons(mServerCPort);

	mBackendServeraddr_in.sin_family = AF_INET;
	mBackendServeraddr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	mBackendServeraddr_in.sin_port = htons(0);

	// setup server A B C sockets ends ----------------------------

	// let user knows it runs
	if (result)
	{
		cout << "The " << mName << " is up and running" << endl;
	}

	return result;
}

void AWSServer::runServer()
{
	Server::runServer();

	char buf[sizeof(ServerMessage)];
	ServerMessage msg2_backend;
	msg2_backend.command = SUM;
	strncpy(msg2_backend.serverName, mName.c_str(), mName.length());

	msg2_backend.entriesCount = 3;
	msg2_backend.data[0] = 1;
	msg2_backend.data[1] = 2;
	msg2_backend.data[2] = 3;
	strncpy(msg2_backend.serverName, mName.c_str(), mName.length());
	msg2_backend.serverName[mName.length()] = '\0';

	memcpy(buf, &msg2_backend, sizeof(msg2_backend));
	sendto(mUDPLocalSockFd, buf, sizeof(buf), 0, (sockaddr*) &mASockaddr_in,
			sizeof(mASockaddr_in));

	bool hasAData = false;
	bool hasBData = false;
	bool hasCData = false;

	long resultA, resultB, resultC;

	socklen_t msg_len = sizeof(struct sockaddr_storage);
	while (recvfrom(mUDPLocalSockFd, buf, sizeof(buf), 0,
			(struct sockaddr*) &mBackendServeraddr_in, &msg_len) > 0)
	{
		// convert buf to servermesage
		ServerMessage msgfrom_backend;
		memcpy(&msgfrom_backend, buf, sizeof(buf));
		string backendName;
		long result;
		string cmdName;

		switch (msg2_backend.command)
		{
		case SUM:
			cmdName = "SUM";
			break;
		case MIN:
			cmdName = "MIN";
			break;
		case MAX:
			cmdName = "MAX";
			break;
		case SOS:
			cmdName = "SOS";
			break;
		default:
			cmdName = "Invalid";
			break;
		}

		// determine the sender
		if (mBackendServeraddr_in.sin_port == htons(UDP_PORT_A))
		{
			// msg from A
			hasAData = msgfrom_backend.command == RESULT
					&& msgfrom_backend.entriesCount == 0;
			resultA = msgfrom_backend.resultValue;
			result = resultA;
		}
		else if (mBackendServeraddr_in.sin_port == htons(UDP_PORT_B))
		{
			// msg from B
			hasBData = msgfrom_backend.command == RESULT
					&& msgfrom_backend.entriesCount == 0;
			resultB = msgfrom_backend.resultValue;
			result = resultB;
		}
		else if (mBackendServeraddr_in.sin_port == htons(UDP_PORT_B))
		{
			// msg from C
			hasCData = msgfrom_backend.command == RESULT
					&& msgfrom_backend.entriesCount == 0;
			resultC = msgfrom_backend.resultValue;
			result = resultC;
		}
		else
		{
			continue;
		}

		// output to console
		backendName = string(msgfrom_backend.serverName);
		cout << "The " << mName << " received reduction result of " << cmdName;
		cout << " from Backend-Server " << backendName << " using UDP over port ";
		cout << ntohs(mBackendServeraddr_in.sin_port) << " and it is " << result << endl;

		// get out of listening mode
		if (hasAData && hasBData && hasCData)
		{
			break;
		}
	}

	//todo add client tcp
}
