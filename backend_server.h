/*
 * backend_server.h
 *
 *  Created on: Oct 2, 2016
 *      Author: Dat Pham
 */

#ifndef BACKEND_SERVER_H_
#define BACKEND_SERVER_H_

#include "server_class.h"

#include <string>
#include <vector>

#include "common.h"

using std::string;
using std::vector;

class BackendServer: public Server
{
public:
	BackendServer(int local_port, int aws_port, string aws_address =
			"localhost", string name = "A");
	virtual ~BackendServer();

	bool initServer();
	void runServer();

private:
	string mAWSAddress;
	vector<long> mIncomingData;
	ServerMessage mMessage;
};

#endif /* BACKEND_SERVER_H_ */