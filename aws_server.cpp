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
	// config TCP port
	mTCPLocalPort = TCP_PORT_AWS;

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
	close(mTCPSockfd);
	close(mRemoteTCPSockfd);
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

	// setup client socket ----------------------------------------
	mTCPSockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (mTCPSockfd < 0)
	{
		result = false;
		perror("Error opening TCP socket");
		exit(EXIT_FAILURE);
	}

	//setup sockaddr_in for local
	memset(&mTCPLocalSockaddr_in, '\0', sizeof(mTCPLocalSockaddr_in));
	mTCPLocalSockaddr_in.sin_family = AF_INET;
	mTCPLocalSockaddr_in.sin_port = htons(mTCPLocalPort);
	mTCPLocalSockaddr_in.sin_addr.s_addr = htonl(INADDR_ANY);

	if (0
			> bind(mTCPSockfd, (struct sockaddr*) &mTCPLocalSockaddr_in,
					sizeof(mTCPLocalSockaddr_in)))
	{
		perror("Error binding TCP socket on AWS");
		result = false;
	}
	// setup client socket ends------------------------------------

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

	while (1)
	{
		// get client msg
		ClientMessage recv_client_msg = getClientMessage();

		// send and get calculated result from backends
		long long final_result = getResultFromBackend(recv_client_msg);

		// output result to console
		cout << "The " << mName << " has successfully finished the reduction "
				<< getClientCalcCommandName(recv_client_msg.command) << ": "
				<< final_result << endl;

		// construct result message to client
		ClientMessage send_client_msg;
		send_client_msg.command = CRESULT;
		send_client_msg.entriesCount = -1;
		send_client_msg.resultValue = final_result;

		// send back to client
		if (!sendClientMessage(send_client_msg))
		{
			cout << "Error sending back result to client" << endl;
		}
		else
		{
			cout << "The " << mName << " has successfully finished sending the "
					<< "reduction value to client" << endl;
		}
	}
}

long long AWSServer::getResultFromBackend(
		const ClientMessage& clientMessage) const
{
	if (clientMessage.entriesCount == 0)
	{
		cout << "Warning: no input data from client" << endl;
		return LLONG_MIN;
	}

	long long result = 0;
	vector<long long> vec_collected_results;

	// determine command from client -------------------------------
	CalcCommand backend_cmd;
	switch (clientMessage.command)
	{
	case CMIN:
		backend_cmd = MIN;
		break;

	case CMAX:
		backend_cmd = MAX;
		break;

	case CSUM:
		backend_cmd = SUM;
		break;

	case CSOS:
		backend_cmd = SOS;
		break;

	default:
		backend_cmd = RESULT;
		break;
	}

	// build command to backend ----------------------------------
	ServerMessage msgA_backend, msgB_backend, msgC_backend;

	// set command
	msgA_backend.command = backend_cmd;
	msgB_backend.command = backend_cmd;
	msgC_backend.command = backend_cmd;

	// set sender name
	strncpy(msgA_backend.serverName, mName.c_str(), mName.length());
	strncpy(msgB_backend.serverName, mName.c_str(), mName.length());
	strncpy(msgC_backend.serverName, mName.c_str(), mName.length());

	msgA_backend.serverName[mName.length()] = '\0';
	msgB_backend.serverName[mName.length()] = '\0';
	msgC_backend.serverName[mName.length()] = '\0';

	// determine data
	for (int i = 0; i < clientMessage.entriesCount; i += 3)
	{
		if (i < clientMessage.entriesCount)
		{
			msgA_backend.data[i / 3] = clientMessage.data[i];
		}

		if (i + 1 < clientMessage.entriesCount)
		{
			msgB_backend.data[i / 3] = clientMessage.data[i + 1];
		}

		if (i + 2 < clientMessage.entriesCount)
		{
			msgC_backend.data[i / 3] = clientMessage.data[i + 2];
		}
	}

	if (clientMessage.entriesCount % 3 == 0)
	{
		msgA_backend.entriesCount = clientMessage.entriesCount / 3;
		msgB_backend.entriesCount = clientMessage.entriesCount / 3;
		msgC_backend.entriesCount = clientMessage.entriesCount / 3;
	}
	else if (clientMessage.entriesCount % 3 == 1)
	{
		msgA_backend.entriesCount = clientMessage.entriesCount / 3 + 1;
		msgB_backend.entriesCount = clientMessage.entriesCount / 3;
		msgC_backend.entriesCount = clientMessage.entriesCount / 3;
	}
	else if (clientMessage.entriesCount % 3 == 2)
	{
		msgA_backend.entriesCount = clientMessage.entriesCount / 3 + 1;
		msgB_backend.entriesCount = clientMessage.entriesCount / 3 + 1;
		msgC_backend.entriesCount = clientMessage.entriesCount / 3;
	}
	// build command to backend ends-------------------------------

	// send command to backends ----------------------------------
	char bufA[sizeof(ServerMessage)];
	char bufB[sizeof(ServerMessage)];
	char bufC[sizeof(ServerMessage)];

	memcpy(bufA, &msgA_backend, sizeof(msgA_backend));
	memcpy(bufB, &msgB_backend, sizeof(msgB_backend));
	memcpy(bufC, &msgC_backend, sizeof(msgC_backend));

	int sentA = sendto(mUDPLocalSockFd, bufA, sizeof(bufA), 0,
			(sockaddr*) &mASockaddr_in, sizeof(mASockaddr_in));
	int sentB =
			(msgB_backend.entriesCount > 0) ?
					sendto(mUDPLocalSockFd, bufB, sizeof(bufB), 0,
							(sockaddr*) &mBSockaddr_in, sizeof(mBSockaddr_in)) :
					-1;
	int sentC =
			(msgC_backend.entriesCount > 0) ?
					sendto(mUDPLocalSockFd, bufC, sizeof(bufC), 0,
							(sockaddr*) &mCSockaddr_in, sizeof(mCSockaddr_in)) :
					-1;

	if (sentA > 0)
	{
		cout << "The " << mName << " has sent " << msgA_backend.entriesCount
				<< " numbers to Backend-Server A" << endl;
	}

	if (sentB > 0)
	{
		cout << "The " << mName << " has sent " << msgB_backend.entriesCount
				<< " numbers to Backend-Server B" << endl;
	}

	if (sentC > 0)
	{
		cout << "The " << mName << " has sent " << msgC_backend.entriesCount
				<< " numbers to Backend-Server C" << endl;
	}

	// send command to backends ends-------------------------------

	bool hasAData = false;
	bool hasBData = (msgB_backend.entriesCount == 0);
	bool hasCData = (msgC_backend.entriesCount == 0);

	long long resultA, resultB, resultC;

	socklen_t msg_len = sizeof(struct sockaddr_storage);
	char buf[sizeof(ServerMessage)];
	while (recvfrom(mUDPLocalSockFd, buf, sizeof(buf), 0,
			(struct sockaddr*) &mBackendServeraddr_in, &msg_len) > 0)
	{
		// convert buf to servermesage
		ServerMessage msgfrom_backend;
		memcpy(&msgfrom_backend, buf, sizeof(buf));
		string backendName;
		long long backend_result = -1;
		string cmdName = getCalcCommandName(backend_cmd);

		// determine the sender
		if (!hasAData && mBackendServeraddr_in.sin_port == htons(mServerAPort))
		{
			// msg from A
			hasAData = msgfrom_backend.command == RESULT
					&& msgfrom_backend.entriesCount == 0;
			resultA = msgfrom_backend.resultValue;
			backend_result = resultA;
			vec_collected_results.push_back(resultA);
		}
		else if (!hasBData
				&& mBackendServeraddr_in.sin_port == htons(mServerBPort))
		{
			// msg from B
			hasBData = msgfrom_backend.command == RESULT
					&& msgfrom_backend.entriesCount == 0;
			resultB = msgfrom_backend.resultValue;
			backend_result = resultB;
			vec_collected_results.push_back(resultB);
		}
		else if (!hasCData
				&& mBackendServeraddr_in.sin_port == htons(mServerCPort))
		{
			// msg from C
			hasCData = msgfrom_backend.command == RESULT
					&& msgfrom_backend.entriesCount == 0;
			resultC = msgfrom_backend.resultValue;
			backend_result = resultC;
			vec_collected_results.push_back(resultC);
		}
		else
		{
			continue;
		}

		// output to console
		backendName = string(msgfrom_backend.serverName);
		cout << "The " << mName << " received reduction result of " << cmdName;
		cout << " from Backend-Server " << backendName
				<< " using UDP over port ";
		cout << ntohs(mBackendServeraddr_in.sin_port) << " and it is "
				<< backend_result << endl;

		// get out of listening mode
		if (hasAData && hasBData && hasCData)
		{
			break;
		}

		//reset buffer
		memset(&buf, 0, sizeof(buf));
	}

	//calculate combined result
	switch (backend_cmd)
	{
	case SUM:
	{
		result = 0;
		for (unsigned i = 0; i < vec_collected_results.size(); ++i)
		{
			result += vec_collected_results.at(i);
		}
	}
		break;
	case MIN:
	{
		result = LLONG_MAX;
		for (unsigned i = 0; i < vec_collected_results.size(); ++i)
		{
			if (result > vec_collected_results.at(i))
			{
				result = vec_collected_results.at(i);
			}
		}
	}
		break;
	case MAX:
	{
		result = LLONG_MIN;
		for (unsigned i = 0; i < vec_collected_results.size(); ++i)
		{
			if (result < vec_collected_results.at(i))
			{
				result = vec_collected_results.at(i);
			}
		}
	}
		break;
	case SOS:
	{
		result = 0;
		for (unsigned i = 0; i < vec_collected_results.size(); ++i)
		{
			result += vec_collected_results.at(i);
		}
	}
		break;
	default:

		break;
	}

	return result;
}

ClientMessage AWSServer::getClientMessage()
{
	ClientMessage result;

	// listening to TCP connection
	listen(mTCPSockfd, 5);
	socklen_t client_len = sizeof(mTCPRemoteSockaddr_in);
	mRemoteTCPSockfd = accept(mTCPSockfd,
			(struct sockaddr*) &mTCPRemoteSockaddr_in, &client_len);
	if (mRemoteTCPSockfd < 0)
	{
		perror("Error on accepting client TCP connection");
	}

	// success TCP handshake with client
	char buf[sizeof(ClientMessage)];
	memset(&buf, 0, sizeof(buf));
	int recv_bytes = read(mRemoteTCPSockfd, buf, 1024);

	if (recv_bytes < 0)
	{
		perror("Error receiving TCP data");
		return result;
	}

	memcpy(&result, buf, sizeof(buf));

	if (result.command == CRESULT)
	{
		cout << "Warning: received result command from client" << endl;
	}

	return result;
}

bool AWSServer::sendClientMessage(const ClientMessage& msg) const
{
	bool result = true;

	char buf[sizeof(ClientMessage)];
	memcpy(buf, &msg, sizeof(msg));
	ssize_t bytes_sent = write(mRemoteTCPSockfd, buf, sizeof(buf));

	if (bytes_sent < 0)
	{
		perror("Error sending to client");
		result = false;
	}

	return result;
}
