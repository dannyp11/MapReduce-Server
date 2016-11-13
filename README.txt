a. Name:	Dat Pham
b. ID:		8012431366

c. What I have done:
	- Successfully implemented server - client system

d. Code structure:
	server_class.h/cpp : 	
		- base class of aws and backend servers. It initiates 1 static local udp fd for sending
		- it also defines message format for communication between aws and backends
			
	backend_server.h/cpp : 
		- backend server (A, B, C) class
		- wait for aws message, then proccesses and sends result back to aws

	aws_server.h/cpp:
		- aws server class

	common.cpp/h:
		- contains configurations like port numbers, standard getopt
		- defines message format for communication between aws and client

	aws.cpp: program that creates and runs an instance of aws_server
	serverABC.cpp: program that creates and runs an instance of backend_server
	client.cpp : client talks with aws, it reads data from ./nums.csv

e. What TA should do:
	make all
	make serverA
	make serverC
	make serverB
	make aws

	./client sum
	./client min
	./client max
	./client sos

f. Format of messages:
	aws <=> backend: a struct in server_class.h contains:
	- an enum command (SUM, SOS, MIN, MAX, RESULT)
	- char* name_of_sender ("A", "AWS",...)
	- long array of fixed size 335
	- int number_of_array_element of the above array
	- long long result_to_be_return

	aws <=> client: a struct in common.h contains:
	- an enum command (CSUM, CSOS, CMIN, CMAX, CRESULT)
	- long array of fixed size 1000
	- int number_of_array_element of the above array
	- long long result_to_be_return
	

g. Issues: 
	- project will fail if binaries run on different host
	- invalid nums.csv only affects client
	- 

h. Reused code:
	- For TCP socket initiating, I consulted TCP server code: http://www.linuxhowtos.org/data/6/server.c
	  but didn't copy its exact content or order. 
	- I pushed the project to my private repository on bitbucket, account datpham@usc.edu. In case plagiarism system detects 			exact similarity
	


