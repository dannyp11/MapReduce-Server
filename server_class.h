/*
 * server_class.h
 *
 *  Created on: Oct 2, 2016
 *      Author: Dat
 *
 * This defines all server behaviors
 *
 */

#ifndef SERVER_CLASS_H_
#define SERVER_CLASS_H_

#include <string>
#include <netinet/in.h>
#include <vector>

using std::string;
using std::vector;

class Server
{
public:
	Server(int remote_port, int local_port, string name = "A");
	virtual ~Server();

	/*
	 *Run listener on server
	 */
	virtual void runServer();

protected:
	/*
	 * Init server A B C
	 */
	virtual bool initServer();

protected:
	bool mIsAws;
	string mName;
	int mUDPRemotePort; // static remote UDP port to talk with servers
	int mUDPLocalPort; // static local UDP port to get data from server

protected:
	// udp variables
	int mUDPRemoteSockFd;
	int mUDPLocalSockFd;
	struct sockaddr_in mServer_Addr;

protected: // data handling
	long getMin(const vector<long>& vec_data);
	long getMax(const vector<long>& vec_data);
	long getSum(const vector<long>& vec_data);
	long getSos(const vector<long>& vec_data);


};
#endif /* SERVER_CLASS_H_ */
