//
//  Server.hpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 4/29/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef Server_hpp
#define Server_hpp

#include "Server_Session.hpp"
#include "Message.hpp"
#include <queue>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Server
{
	private:
		tcp::endpoint endpoint;
		tcp::acceptor acc;
		tcp::socket socket;
		
	public:
		Server(boost::asio::io_service& io_service, int port, std::queue<q_elt>* mesQ);
		Server(Server&) = delete;
		virtual ~Server(void);
		
	private:
		void run(std::queue<q_elt>* mesQ);
};

#endif /* Server_hpp */
