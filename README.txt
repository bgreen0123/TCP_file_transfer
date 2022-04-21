Relavent files:
	DieWithError.h: This is a header file that will simply throw an error message if something unexpected occurs.
	HandleTCPClient.h: This is a header file that takes care of finding the requested file and transfering the data to the client.
	TCPMsgClient.c: This is the file that manages the client side. It creates the socket, connects it to the server, requests the file, and retevies the data sent from the server.
	TCPMsgServer.c: This is file that manages the server side. It will create the the server socket, bind it to the client socket, and have it start listening for requests. Once it finds the request from the client it will call HandTCPClient from the header file and send the client that files data. 
	makefile: This file allows easy setup in configuring the project. To make the entire project simply type make all. To get it back to its original form type make clean.

Compliation instructions:
	1.) Use the makefile to remove anything that might be lingering: make clean
	2.) Use the makefile to build all the required executables: make all
	3.) Start up the server side so it is listening: ./TCPMsgServer
	4.) Start up the client side including the ip address: ./TCPMsgClient <ip add>
	5.) Enter the file you want to retrieve from the server: <File Name>
	6.) The file will be located under out.txt.

Configuration files:
	makefile: This allows for easy configuration of the project, and will compile everything into a runnable state. It is setup so simply typing make all will compile everything needed and typing make clean will remove all exacutables.

Running Instructions:
	TCPMsgClient: This is the executable for the client side. In order to run it you simply type ./TCPMsgClient
	TCPMsgServer: This is the executable for the server side. In order to run it you simply type ./TCPMsgServer
	Command Line Arguments: Neither of these take in any arguments as both the host IP address and the port are hard coded.
