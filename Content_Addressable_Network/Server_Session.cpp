//
//  Server_Session.cpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#include "Server_Session.hpp"
#include <iostream>

Server_Session::Server_Session(tcp::socket socket) : socket_(std::move(socket))
{
    
}

Server_Session::~Server_Session()
{
    
}

void Server_Session::start(std::queue<q_elt>* mesQ)
{
    do_read(mesQ);
}

void Server_Session::do_read(std::queue<q_elt>* mesQ)
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(buf, max_length),
                            [this, self, mesQ](boost::system::error_code ec, std::size_t length)
                            {
                                if (!ec)
                                {
                                    q_elt element(buf.data(), max_length);
                                    mesQ->emplace(element);
                                }
                                do_read(mesQ);
                            });
    //auto q_elt(buf.data(), max_length);
    //mesQ.emplace(q_elt);
}

void Server_Session::do_write(std::size_t length)
{
    
}
