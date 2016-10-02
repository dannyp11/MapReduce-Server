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
#include   <netinet/in.h>

using std::string;

class Server
{
public:
	Server(string address, int port_number, string name = "AWS");
	virtual ~Server();

protected:
	/*
	 * Init server A B C
	 */
	void initServer();

	/*
	 * Init aws server
	 */
	void initServerAWS();

protected:
	bool mIsAws;
	string mName;
	string mAddress;
	int mPort;

protected: // udp variables
	int mSockFd;
	struct sockaddr_in	mServer_Addr;


};

#endif /* SERVER_CLASS_H_ */
