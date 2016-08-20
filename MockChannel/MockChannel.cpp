/////////////////////////////////////////////////////////////////////
//  MockChannel.cpp                                                //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell XPS 15, Windows 7 Home Edition             //
//  Application:   Dependency Based Remote Code Repository for     //
//                 CSE687 Pr4, Sp16                                //
//  Author:		   Sushant Garg, Syracuse University		       //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#define IN_DLL
#include "MockChannel.h"
#include <string>
#include "../MsgClient/MsgClient.h"
#include <iostream>

/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class MockChannel : public IMockChannel
{
public:
	MockChannel(std::string ip, int port);
	void start();
	void postMessage(const std::string& msg, const std::string& body);
	std::string getMessage() { return client_.getMessage(); }
private:
	MsgClient client_;
	bool stop_ = false;
};

//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(std::string ip, int port) : client_(ip,port){}

//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start()
{
  std::cout << "\n  MockChannel starting up";
  client_.start();
}

void MockChannel::postMessage(const std::string& msg, const std::string& body) 
{ 
	HttpMessage messge;

	messge.addAttribute(HttpMessage::attribute("COMMAND", msg));
	
	messge.addBody(body);
	//std::cout << "\n\n  Command : " + msg + "\n  body: " + body + "\n";
	//std::cout << "\n  HttpMESSAGE: " + messge.toString();
	client_.postHttpMessage(messge);
}

IMockChannel* ObjectFactory::createMockChannel(std::string ip, int port)
{ 
  return new MockChannel(ip,port); 
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
  ObjectFactory objFact;
  ISendr* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
  pMockChannel->start();
  pSendr->postMessage("Hello World");
  pSendr->postMessage("CSE687 - Object Oriented Design");
  Message msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pSendr->postMessage("stopping");
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pMockChannel->stop();
  pSendr->postMessage("quit");
  std::cin.get();
}
#endif
