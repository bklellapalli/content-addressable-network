//
//  Server_Session.cpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#include "Server_Session.hpp"
#include <iostream>



session::session(tcp::socket socket) : socket_(std::move(socket))
{
}

session::~session()
{
}

void session::start(std::queue<q_elt>* mesQ)
{
    do_read(mesQ);
}

void session::do_read(std::queue<q_elt>* mesQ)
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

void session::do_write(std::size_t length)
{
    
}
