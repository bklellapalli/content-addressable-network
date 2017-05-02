//
//  Node.hpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17
//  Copyright © 2017 Balakrishna. All rights reserved.

#ifndef NODE_HPP
#define NODE_HPP

#include "Server.hpp"
#include "Member.hpp"
#include <boost/asio.hpp>
#include <iostream>

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
