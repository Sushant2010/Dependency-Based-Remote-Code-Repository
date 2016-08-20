#ifndef SERVER_H
#define SERVER_H

/////////////////////////////////////////////////////////////////////
//  Server.h -  Package used to define functions in Server         //
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
This module is for accepting files & storing in repository.


Public Interface:
================
ClientHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
void operator()(Socket socket)
HttpMessage readMessage(Socket& socket);
HttpMessage parseFileMsg(Socket& socket, HttpMessage message);
bool readFile(const std::string& filename, size_t fileSize,const std::string& depend, Socket& socket);
void buildXML(std::string path, std::string file, std::string depend);
void sendMessage(HttpMessage& msg, Socket& socket);
bool sendFile(const std::string& fqname, Socket& socket);
std::string getDependency(std::string filepath);

Build Process:
==============
Required files
- HttpMessage.h
- Sockets.h
- FileSystem.h
- Logger.h
- Utilities.h
- DataStore.h
- XmlElement.h
- XmlParser.h

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
#include "../FileSystem/FileMgr.h"
#include "../FileSystem/DataStore.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "XmlElement.h"
#include "../XmlParser/XmlParser.h"
#include <string>
#include <iostream>
#include <ctime>

using Show = StaticLogger<1>;
using namespace Utilities;
using namespace XmlProcessing;


/////////////////////////////////////////////////////////////////////
// ClientHandler class
/////////////////////////////////////////////////////////////////////
// - instances of this class are passed by reference to a SocketListener
// - when the listener returns from Accept with a socket it creates an
//   instance of this class to manage communication with the client.
// - You need to be careful using data members of this class
//   because each client handler thread gets a reference to this 
//   instance so you may get unwanted sharing.
// - I may change the SocketListener semantics (this summer) to pass
//   instances of this class by value.
// - that would mean that all ClientHandlers would need either copy or
//   move semantics.
//
class ClientHandler
{
public:
	ClientHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);
private:
	bool connectionClosed_;
	std::string serverPath_ = "../ServerRoot/";
	HttpMessage readMessage(Socket& socket);
	std::string getFiles();
	HttpMessage parseFileMsg(Socket& socket, HttpMessage message);
	bool readFile(const std::string& filename, size_t fileSize,const std::string& depend, Socket& socket);
	void buildXML(std::string path, std::string file, std::string depend);
	BlockingQueue<HttpMessage>& msgQ_;
};

class MsgServer
{
public:
	using EndPoint = std::string;
	MsgServer(BlockingQueue<HttpMessage>& q) : queue(q),serverReceiver(queue) {}//ip_(ip), port_(port), serverReceiver(queue) {}
	void execute();
	void startSenderThread();
	void startReceiverThread();
	void processRequest(SocketConnecter& si, HttpMessage& request);
	BlockingQueue<HttpMessage>& getServerQ() { return queue; }
private:
	BlockingQueue<HttpMessage> queue;
	void startSender();
	void startReceiver();
	std::vector<std::string> recursiveDependency(std::string file);
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const std::string& fqname, Socket& socket);
	std::string getDependency(std::string filepath);
	ClientHandler serverReceiver;
};

#endif
//BlockingQueue<HttpMessage> gblQ;