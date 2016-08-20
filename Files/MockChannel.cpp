/////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - Demo for CSE687 Project #4, Spring 2015               //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - MockChannel reads from sendQ and writes to recvQ                      //
//                                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015               //
/////////////////////////////////////////////////////////////////////////////

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
	client_.postHMessage(messge);
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
