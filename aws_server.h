/*
 * aws_server.h
 *
 *  Created on: Oct 22, 2016
 *      Author: dat
 */

#ifndef AWS_SERVER_H_
#define AWS_SERVER_H_

#include "server_class.h"
#include "common.h"

using std::string;
using std::vector;

class AWSServer :public Server
{
public:
	AWSServer();
	virtual ~AWSServer();

	bool initServer();
	void runServer();

protected:
	long long getResultFromBackend(const ClientMessage& clientMessage) const;

	// get message from client
	ClientMessage getClientMessage();

	bool sendClientMessage(const ClientMessage& msg) const;

private:
	// for backend servers
	int mServerAPort;
	int mServerBPort;
	int mServerCPort;
	struct hostent* mServerA;
	struct hostent* mServerB;
	struct hostent* mServerC;
	string mServerAAddress;
	string mServerBAddress;
	string mServerCAddress;
	struct sockaddr_in mASockaddr_in;
	struct sockaddr_in mBSockaddr_in;
	struct sockaddr_in mCSockaddr_in;
	struct sockaddr_in mBackendServeraddr_in;


	// for client
	struct hostent* mClient;
	int mTCPSockfd;
	int mRemoteTCPSockfd;
	int mTCPLocalPort;
	struct sockaddr_in mTCPRemoteSockaddr_in;
	struct sockaddr_in mTCPLocalSockaddr_in;
};

#endif /* AWS_SERVER_H_ */
