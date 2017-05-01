#ifndef NODE_HPP
#define NODE_HPP

#include "server.hpp"
#include "Member.hpp"
#include <boost/asio.hpp>
#include <iostream>
class Node : public Member
{
	private:
		Server server;
	
	public:
		Node(boost::asio::io_service& io_service, int port) : server {io_service, port, mesQ} { } 
		void getMessages();
		void init_mem_protocol(void);	//thread
};

#endif
