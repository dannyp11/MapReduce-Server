/*
 * backend_server.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Dat Pham
 */

#include "backend_server.h"

#include <iostream>

using std::cout;
using std::endl;

BackendServer::BackendServer(int local_port, int aws_port, string aws_address,
		string name) :
		Server(aws_port, local_port, name), mAWSAddress(aws_address)
{

	mIsAws = false;

	initServer();
}

BackendServer::~BackendServer()
{
	// TODO Auto-generated destructor stub
}

bool BackendServer::initServer()
{
	bool res = Server::initServer();

	// let user knows it runs
	cout << "The Server " << mName << " is up and running using UDP on port "
			<< mUDPLocalPort << endl;

	return res;
}

void BackendServer::runServer()
{
}
