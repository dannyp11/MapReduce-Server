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

const int BUF_LEN = 65534;
const int MAX_UDP_ENTRIES = 350;
const int NAME_LEN = 256;

// Message Packet specificatino --------------------
// this is used for server communicating with each other
typedef enum e_CalcCommand
{
	MIN, MAX, SUM, SOS, RESULT
} CalcCommand;

typedef struct s_ServerMessage
{
	// header
	char serverName[NAME_LEN];
	CalcCommand command;

	// payload
	int entriesCount;
	long data[MAX_UDP_ENTRIES]; // must be null if sent from A, B, C. Limit to 350 values
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

	/*
	 * Simple CalcCommand name converter
	 */
	string getCalcCommandName(CalcCommand cmd) const;

protected:
	bool mIsAws;
	string mName;
	int mUDPLocalPort; // static local UDP port to get data from server

protected:
	// udp variables
	int mUDPLocalSockFd;
	struct sockaddr_in mUDPLocalAddr_in;

protected:
	// data handling
	long getMin(const vector<long>& vec_data) const;
	long getMax(const vector<long>& vec_data) const;
	long getSum(const vector<long>& vec_data) const;
	long getSos(const vector<long>& vec_data) const;

	vector<long> getVectorFromArray(int data_len, const long data[]) const;

private:
	bool isInitSuccess;
};
#endif /* SERVER_CLASS_H_ */
