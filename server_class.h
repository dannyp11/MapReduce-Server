/*
 * server_class.h
 *
 *  Created on: Oct 2, 2016
 *      Author: Dat
 *
 * This defines all server behaviors, base Server class
 *
 */

#ifndef SERVER_CLASS_H_
#define SERVER_CLASS_H_

#include <string>
#include <netinet/in.h>
#include <vector>

using std::string;
using std::vector;

const int BUF_LEN = 1024;

// Message Packet specificatino --------------------
// this is used for server communicating with each other

typedef enum e_CalcCommand
{
	MIN, MAX, SUM, SOS, RESULT
} CalcCommand;

typedef struct s_ServerMessage
{
	std::string serverName;
	CalcCommand command;
	std::vector<long> data; // must be null if sent from A, B, C. Limit to 100values
	long resultValue; // must be LONG_MIN if sent from AWS
} ServerMessage;
// ----------------------------------------

class Server
{
public:
	Server(int local_port, string name = "A"); // ports are static udp
	virtual ~Server();

	/*
	 *Run the server, this will usually be infinite loop
	 */
	virtual void runServer() = 0;

protected:
	/*
	 * Init server's UDP listener
	 */
	virtual bool initServer();

protected:
	bool mIsAws;
	string mName;
	int mUDPLocalPort; // static local UDP port to get data from server

protected:
	// udp variables
	int mUDPLocalSockFd;
	struct sockaddr_in mUDPLocalAddr_in;

protected: // data handling
	long getMin(const vector<long>& vec_data);
	long getMax(const vector<long>& vec_data);
	long getSum(const vector<long>& vec_data);
	long getSos(const vector<long>& vec_data);


};
#endif /* SERVER_CLASS_H_ */
