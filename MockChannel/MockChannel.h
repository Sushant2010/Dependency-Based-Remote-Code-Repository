#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
/////////////////////////////////////////////////////////////////////////////
//  MockChannel.h -                                                        //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - MockChannel reads from sendQ and writes to recvQ                      //
//  ver 1.0                                                                //
//  Language:      Visual C++ 2008, SP1									   //
//  Platform:      Dell XPS 15, Windows 7 Home Edition                     //
//  Application:   Dependency Based Remote Code Repository for             //
//                 CSE687 Pr4, Sp16                                        //
//  Author:		   Sushant Garg, Syracuse University		               //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University             //
//                 (315) 443-3948, jfawcett@twcny.rr.com                   //
/////////////////////////////////////////////////////////////////////////////

/*

Module Operations:
==================
This module is for building .dll


Build commands
- devenv CommPrototype.sln

Maintenance History:
====================
ver 1.0 : 4 May 16
- first release

*/

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif
#include <string>

struct IMockChannel
{
public:
	virtual std::string getMessage() = 0;
	virtual void postMessage(const std::string& msg, const std::string& body) = 0;
	virtual void start() = 0;
};

extern "C" {
  struct ObjectFactory
  {
    DLL_DECL IMockChannel* createMockChannel(std::string ip, int port);
  };
}


#endif


