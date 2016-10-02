/*
 * server_class.h
 *
 *  Created on: Oct 2, 2016
 *      Author: danny
 *
 * This defines all server behaviors
 *
 */

#ifndef SERVER_CLASS_H_
#define SERVER_CLASS_H_

class Server {
public:
	/*
	 * name of back end server
	 */
	typedef enum e_ServerName{
		A = 'A',
		B = 'B',
		C = 'C',
		AWS
	} ServerName;

	Server(ServerName name = AWS);
	virtual ~Server();

private:
	bool mIsAws;
	ServerName mName;

};

#endif /* SERVER_CLASS_H_ */
