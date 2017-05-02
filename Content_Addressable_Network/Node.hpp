#ifndef NODE_HPP
#define NODE_HPP

#include "Server.hpp"
#include "Member.hpp"
#include <boost/asio.hpp>
#include <iostream>
 
 /* STRUCT NAME: MessageHdr
 *
 * DESCRIPTION: Header and content of a message */

class Node : public Member
{
	private:
		Server server;
	
	public:
        Node(boost::asio::io_service& io_service, int port);
        void getMessages();
		void init_mem_protocol(void);	//thread
        void performOperation();
};

#endif
