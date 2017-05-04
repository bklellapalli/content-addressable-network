//
//  Server_Session.hpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef SERVER_SESSION_HPP
#define SERVER_SESSION_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "Member.hpp"

using boost::asio::ip::tcp;

class Server_Session : public std::enable_shared_from_this<Server_Session>
{
    private:
        enum { max_length = 1024 };
	  	boost::array<char, max_length> buf;
	  	tcp::socket socket_;
    
        void do_read(std::queue<q_elt>* mesQ);
        void do_write(std::size_t);

    public:
	  	Server_Session(tcp::socket socket);
	  	~Server_Session();
	  	void start(std::queue<q_elt>* mesQ);
};

#endif /* SERVER_SESSION_HPP */
