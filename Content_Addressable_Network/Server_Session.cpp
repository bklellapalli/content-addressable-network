
/**********************************
 * FILE NAME: Server_Session.cpp
 *
 * Created by Shriram Joshi on 5/1/17
 * DESCRIPTION: Definition of all Server_Session related class
 *
 * Copyright © 2017 Balakrishna. All rights reserved.
 **********************************/

#include "Server_Session.hpp"
#include "Logger.hpp"

Server_Session::Server_Session(tcp::socket socket) : socket_(std::move(socket)) 
{
}

Server_Session::~Server_Session() { }

void Server_Session::start(SharedQueue<q_elt>* mesQ)
{
    do_read(mesQ);
}

void Server_Session::do_read(SharedQueue<q_elt>* mesQ)
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(buf, max_length),
    [this, self, mesQ](boost::system::error_code ec, std::size_t bytes_transferred)
    {
        if (!ec)
        {
        	std::string data;
        	std::copy(buf.begin(), buf.begin() + bytes_transferred, std::back_inserter(data));
            q_elt el(data, bytes_transferred);
            mesQ->push_back(el);
        }
        do_read(mesQ);
	});
}


//void Server_Session::do_write(std::size_t length) { }
