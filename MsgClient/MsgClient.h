#ifndef MSGCLIENT_H
#define MSGCLIENT_H

/////////////////////////////////////////////////////////////////////
//  MsgClient.h -  Sending files to server from the Client         //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell XPS 15, Windows 7 Home Edition             //
//  Application:   Dependency Based Remote Code Repository for     //
//                 CSE687 Pr4, Sp16                                //
//  Author:		   Sushant Garg, Syracuse University		       //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*

Module Operations:
==================
This module is for sending files to Server from the Client


Public Interface:
================
HandClient(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
void operator()(Socket socket);
HttpMessage readMessage(Socket& socket);
bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
MsgClient(std::string ip, int port):ip_(ip),port_(port),clientReceiver(rcvQueue) {}
bool Connect(SocketConnecter& si, std::string ipAddress, int port);
void processRequest(SocketConnecter& si, std::string msg, std::string body);
void MsgClient::postHMessage(const HttpMessage& msg);
HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
void sendMessage(HttpMessage& msg, Socket& socket);
bool sendFile(const std::string& fqname, const std::string& dep, Socket& socket);


Build Process:
==============
Required files
- HttpMessage.h
- Sockets.h
- FileSystem.h
- Logger.h
- Utilities.h

Build commands
- devenv CommPrototype.sln

Maintenance History:
====================
ver 1.0 : 4 May 16
- first release

*/


#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include <string>
#include <iostream>

using Show = StaticLogger<1>;
using namespace Utilities;
using Utils = StringHelper;


class HandleClient
{
public:
	HandleClient(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);
private:
	bool connClosed_;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	BlockingQueue<HttpMessage>& msgQ_;
};

/////////////////////////////////////////////////////////////////////
// MsgClient class
// - was created as a class so more than one instance could be 
//   run on child thread
//
class MsgClient
{
public:
	using EndPoint = std::string;
	MsgClient(std::string ip, int port):ip_(ip),port_(port),clientReceiver(receiveQueue) {}
	void start();
	void startSender();
	bool Connect(SocketConnecter& si, std::string ipAddress, int port);
	void processRequest(SocketConnecter& si, std::string msg, std::string body);
	void MsgClient::postHttpMessage(const HttpMessage& msg);
	void startReceiver();
	void startSenderThread();
	void startReceiverThread();
	std::string MsgClient::getMessage(); //{ return rcvQueue.deQ(); }
	BlockingQueue<HttpMessage> getQ() { return sendQueue; }
private:
	EndPoint endpoint;
	std::string ip_;
	int port_;
	BlockingQueue<HttpMessage> sendQueue;
	BlockingQueue<HttpMessage> receiveQueue;
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const std::string& fqname, const std::string& dep, Socket& socket);
	HandleClient clientReceiver;
};

#endif