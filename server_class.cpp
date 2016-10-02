/*
 * server_class.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: Dat
 */

#include "server_class.h"

Server::Server(ServerName name) : mName(name) {

	if (name == AWS)
	{
		mIsAws = true;
	}
	else
	{
		mIsAws = false;
	}

}

Server::~Server() {
	// TODO Auto-generated destructor stub
}

