///////////////////////////////////////////////////////////////////////
//  MsgServer.cpp - Receives files from the Client                   //
//  Language:      Visual C++ 2008, SP1                              //
//  Platform:      Dell XPS 15, Windows 7 Home Edition               //
//  Application:   Dependency Based Remote Code Repository for       //
//                 CSE687 Pr4, Sp16                                  //
//  Author:		   Sushant Garg, Syracuse University		         //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University       //
//                 (315) 443-3948, jfawcett@twcny.rr.com             //
///////////////////////////////////////////////////////////////////////
/*
* This package implements a server that receives HTTP style messages and
* files from multiple concurrent clients and simply displays the messages
* and stores files.
*
* It's purpose is to provide a very simple illustration of how to use
* the Socket Package provided for Project #4.
*/
/*
* Required Files:
*   MsgClient.cpp, MsgServer.cpp
*   HttpMessage.h, HttpMessage.cpp
*   Cpp11-BlockingQueue.h
*   Sockets.h, Sockets.cpp
*   FileSystem.h, FileSystem.cpp
*   Logger.h, Logger.cpp 
*   Utilities.h, Utilities.cpp
*/

#include "Server.h"

void ClientHandler::buildXML(std::string path, std::string file_,std::string depend)
{
	using sPtr = std::shared_ptr < AbstractXmlElement >;
	sPtr mtdata = makeTaggedElement("metadata");
	sPtr name1 = makeTaggedElement("name");
	mtdata->addChild(name1);
	name1->addChild(makeTextElement(file_));
	sPtr deps = makeTaggedElement("deps");
	mtdata->addChild(deps);
	size_t pos = 0;
	while (pos != std::string::npos)
	{
		pos = depend.find(',');
		if (pos != std::string::npos)
		{
			sPtr dep1 = makeTaggedElement("dep");
			deps->addChild(dep1);
			dep1->addChild(makeTextElement(depend.substr(0, pos)));
			depend.erase(0, pos + 1);
		}
	}
	sPtr author = makeTaggedElement("author");
	mtdata->addChild(author);
	author->addChild(makeTextElement("Client1"));
	sPtr closed = makeTaggedElement("closed");
	mtdata->addChild(closed);
	closed->addChild(makeTextElement("true"));
	sPtr docE1 = makeDocElement(mtdata);
	if (FileSystem::Directory::exists(path))
	{
		FileSystem::File mtdata(path + "/metadata.xml");
		mtdata.open(FileSystem::File::out, FileSystem::File::binary);
		std::string mtdataString = docE1->toString();
		const size_t siz = mtdataString.size();
		FileSystem::Block blk;
		FileSystem::Byte tab2[1024];
		strncpy(tab2, mtdataString.c_str(), sizeof(tab2));
		tab2[sizeof(tab2) - 1] = 0;

		for (size_t i = 0; i < siz; i++)
			blk.push_back(tab2[i]);

		mtdata.putBlock(blk);
		mtdata.close();
	}
	std::cout << "\n MetaData for " + file_ + "\n" << docE1->toString();
	std::cout << "\n\n";
}

 //----< this defines processing to frame messages >------------------

HttpMessage ClientHandler::readMessage(Socket& socket)
{
	connectionClosed_ = false;
	HttpMessage msg;
	while (true)
	{
		std::string attribString = socket.recvString('\n');
		if (attribString.size() > 1)
		{
			HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);
			msg.addAttribute(attrib);
		}
		else		
			break;
	}
	// If client is done, connection breaks and recvString returns empty string
	if (msg.attributes().size() == 0) {
		connectionClosed_ = true;
		return msg;
	}
	// read body if POST - all messages in this demo are POSTs
	std::cout << "\n\nServer received : \n" + msg.toString() + "\n";
	if (msg.attributes()[0].first == "POST")
	{
		// is this a file message?
		std::string command = msg.findValue("COMMAND");
		std::string strfiles;
		if (command == "CHECKIN")
		{
			return parseFileMsg(socket, msg);			
		}
		else if (command == "GETFILES")
		{
			strfiles = getFiles();
			msg.removeAttribute("toAddr");
			msg.removeAttribute("fromAddr");
			msg.addAttribute(HttpMessage::Attribute("toAddr", "9080"));
			msg.addAttribute(HttpMessage::Attribute("fromAddr", "9081"));
			msg.addAttribute(HttpMessage::Attribute("getList", strfiles));
		}
		else if (command == "EXTRACT")
			return msg;
	}
	return msg;
}

HttpMessage ClientHandler::parseFileMsg(Socket& socket, HttpMessage message)
{
	std::string filename = message.findValue("file");
	std::string dependency = message.findValue("DEPENDENCY");

	if (filename != "")
	{
		size_t contentSize;
		std::string sizeString = message.findValue("content-length");
		if (sizeString != "")
			contentSize = Converter<size_t>::toValue(sizeString);
		else
			return message;

		readFile(filename,contentSize,dependency, socket);
	}

	if (filename != "")
	{
		// construct message body

		message.removeAttribute("content-length");
		std::string bodyString = "<file>" + filename + "</file>";
		std::string sizeString = Converter<size_t>::toString(bodyString.size());
		message.addAttribute(HttpMessage::Attribute("content-length", sizeString));
		message.addBody(bodyString);
	}
	else
	{
		// read message body

		size_t numBytes = 0;
		size_t pos = message.findAttribute("content-length");
		if (pos < message.attributes().size())
		{
			numBytes = Converter<size_t>::toValue(message.attributes()[pos].second);
			Socket::byte* buffer = new Socket::byte[numBytes + 1];
			socket.recv(numBytes, buffer);
			buffer[numBytes] = '\0';
			std::string msgBody(buffer);
			message.addBody(msgBody);
			delete[] buffer;
		}
	}
}

//----< read a binary file from socket and save >--------------------
bool ClientHandler::readFile(const std::string& filename, size_t fileSize, const std::string& depend, Socket& socket)
{
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	std::string year = Converter<int>::toString(now->tm_year + 1900);
	std::string mon = Converter<int>::toString(now->tm_mon + 1);
	std::string day = Converter<int>::toString(now->tm_mday);
	std::string hour = Converter<int>::toString(now->tm_hour);
	std::string minute = Converter<int>::toString(now->tm_min);
	std::string second = Converter<int>::toString(now->tm_sec);
	std::string fname = Utilities::StringHelper::trimFileName(filename);
	std::string time = "(" + year + "-" + mon + "-" + day + " " + hour + "-" + minute + "-" + second + ")";
	std::string dir = serverPath_ + fname + " " + time;
	FileSystem::Directory::create(dir);

	std::string fqname = dir + "/" + fname + ".snt";
	FileSystem::File file(fqname);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood())
	{
		Show::write("\n\n  can't open file " + fqname);
		return false;
	}

	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];

	size_t bytesToRead;
	while (true)
	{
		if (fileSize > BlockSize)
			bytesToRead = BlockSize;
		else
			bytesToRead = fileSize;

		socket.recv(bytesToRead, buffer);

		FileSystem::Block blk;
		for (size_t i = 0; i < bytesToRead; ++i)
			blk.push_back(buffer[i]);

		file.putBlock(blk);
		if (fileSize < BlockSize)
			break;
		fileSize -= BlockSize;
	}
	file.close();
	buildXML(dir, filename,depend);
	return true;
}

std::string ClientHandler::getFiles()
{
	std::string stringFiles;
	DataStore ds;
	FileMgr fm(serverPath_, ds);
	fm.search();
	std::vector<std::string> vecFiles = fm.getFiles();

	for each (auto str in vecFiles)
	{
		stringFiles.append(str);
		stringFiles.append(",");
	}
	return stringFiles;
}

//----< receiver functionality is defined by this function >---------
void ClientHandler::operator()(Socket socket)
{
	
	while (true)
	{
		//std::cout << "\n\n in server operator()";
		HttpMessage msg = readMessage(socket);
		if (connectionClosed_ || msg.bodyString() == "quit")
		{
			Show::write("\n\n  clienthandler thread is terminating");
			break;
		}
		msgQ_.enQ(msg);
	}
}

HttpMessage MsgServer::makeMessage(size_t n, const std::string& body, const EndPoint& ep)
{
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	EndPoint myEndPoint = "localhost:8081";  

	switch (n)
	{
	case 1:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("POST", "Message"));
		msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
		msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
		msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));

		msg.addBody(body);
		if (body.size() > 0)
		{
			attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
			msg.addAttribute(attrib);
		}
		break;
	default:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
	}
	return msg;
}

//----< send message using socket >----------------------------------
void MsgServer::sendMessage(HttpMessage& msg, Socket& socket)
{
	std::string msgString = msg.toString();
	//std::cout << "\n\nServer received : \n" + msgString + "\n";
	socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}

//----< send file using socket >-------------------------------------
bool MsgServer::sendFile(const std::string& filename, Socket& socket)
{
	FileSystem::FileInfo fi(filename);
	size_t fileSize = fi.size();
	std::string sizeString = Converter<size_t>::toString(fileSize);
	FileSystem::File file(filename);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
		return false;

	HttpMessage msg = makeMessage(1, "", "localhost::9080");
	msg.addAttribute(HttpMessage::Attribute("file", Utilities::StringHelper::trimFileName(filename)));
	msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
	sendMessage(msg, socket);
	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];
	while (true)
	{
		FileSystem::Block blk = file.getBlock(BlockSize);
		if (blk.size() == 0)
			break;
		for (size_t i = 0; i < blk.size(); ++i)
			buffer[i] = blk[i];
		socket.send(blk.size(), buffer);
		if (!file.isGood())
			break;
	}
	file.close();
	return true;
}

std::string MsgServer::getDependency(std::string filepath)
{
	XmlParser parser(filepath);
	parser.verbose();
	XmlDocument* pDoc = parser.buildDocument();
	return parser.returnDependency();
}

//----< this defines the behavior of the client >--------------------
void MsgServer::execute()
{
	Show::attach(&std::cout);
	Show::start();
	Show::title(
		"Starting HttpMessage server on thread " + Utilities::Converter<std::thread::id>::toString(std::this_thread::get_id())
		);
	try
	{
		SocketSystem ss;
		SocketConnecter si;
		while (!si.connect("localhost", 9080))
		{
			Show::write("\n server waiting to connect");
			::Sleep(100);
		}

		// send a set of messages

		HttpMessage msg;
		std::vector<std::string> files = FileSystem::Directory::getFiles("../TestFiles", "*.h");
		for (size_t i = 0; i < files.size(); ++i)
		{
			Show::write("\n\n  sending file " + files[i]);
			sendFile(files[i], si);
		}

		// shut down server's client handler

		msg = makeMessage(1, "quit", "toAddr:localhost:8080");
		sendMessage(msg, si);

		Show::write("\n");
		Show::write("\n  All done folks");
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

void MsgServer::processRequest(SocketConnecter& si, HttpMessage& request)
{
	while (!si.connect("localhost", 9080))
	{
		Show::write("\n server waiting to connect");
		::Sleep(100);
	}
	std::string command = request.findValue("COMMAND");
	std::string dependency = "";
	if(command == "EXTRACT")
	{
		std::cout << "\n EXTRACT command received..\n";
		std::string file = request.findValue("file");
		std::string checked = request.findValue("CHECKED");
		std::vector<std::string> vecDepend;
		if (checked == "1")
		{
			std::vector<std::string> temp = recursiveDependency(file);
			vecDepend.insert(vecDepend.end(), temp.begin(), temp.end());
		
		}
		
			for (std::string str : vecDepend)
				sendFile(str, si);
		
		sendFile(file, si);
		return;
	}
	sendMessage(request, si);
}

std::vector<std::string> MsgServer::recursiveDependency(std::string file)
{
	const size_t last = file.find_last_of("\\/");
	std::string metadata;
	std::vector<std::string> vecDepend;
	if (std::string::npos != last)
	{
		metadata = file.substr(0, last + 1) + "\\metadata.xml";
	}
	std::string dependency = getDependency(metadata);
	std::cout << "\n\n Dependency from XML Parser: " + dependency + "\n\n";

	if (dependency != "")
	{
		size_t pos = 0;
		do
		{
			pos = dependency.find(',');
			std::string depFile = dependency.substr(0, pos);
			int excess = depFile.find_last_of('\n');
			if (excess > 0)
			{
				depFile.erase(excess, depFile.size());
				vecDepend.push_back(depFile);
				dependency.erase(0, pos + 1);
			}
		} while (pos != std::string::npos);
		for (std::string str : vecDepend) 
		{
			std::vector<std::string> temp = recursiveDependency(str);
			for (std::string tmp : temp)
			{
				if (std::find(vecDepend.begin(), vecDepend.end(), tmp) != vecDepend.end())
					vecDepend.push_back(tmp);
			}
			
		}
	}
	return vecDepend;
}

void MsgServer::startSender()
{
	try
	{
		SocketSystem ss;
		SocketConnecter si;
		while (true)
		{
			auto request = queue.deQ();
			processRequest(si, request);
		}
	}
	catch (std::exception& ex)
	{
		Show::write("  Exception caught:");
		Show::write(std::string("\n  ") + ex.what() + "\n\n");
	}
}

void MsgServer::startReceiver()
{
	try
	{
		SocketSystem ss;
		SocketListener sl(8080, Socket::IP6);
		sl.start(serverReceiver);

		Show::write("press key to exit: ");
		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception& ex)
	{
		Show::write("  Exception caught:");
		Show::write(std::string("\n  ") + ex.what() + "\n\n");
	}
}

void MsgServer::startSenderThread()
{
	std::thread receiverThread(&MsgServer::startSender, this);
	receiverThread.detach();
}

void MsgServer::startReceiverThread()
{
	std::thread receiverThread(&MsgServer::startReceiver, this);
	receiverThread.detach();
}

//----< test stub >--------------------------------------------------

int main()
{
	::SetConsoleTitle(L"HttpMessage Server");
	Show::attach(&std::cout);
	Show::start();
	Show::title("\n  HttpMessage Server has started");

	BlockingQueue<HttpMessage> msgQ;

	try
	{
		MsgServer server_(msgQ);
		server_.startReceiverThread();
		server_.startSenderThread();

		while (true)
		{
			//HttpMessage msg = msgQ.deQ();
			//Show::write("\n\n  server recvd message contents:\n" + msg.bodyString());
		}
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}