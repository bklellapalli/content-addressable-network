//
//  Server.cpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#include "Server.hpp"
#include "Server_Session.hpp"
#include <iostream>

Server::~Server()
{
    
}

void Server::run(std::queue<q_elt>* mesQ)
{
    acc.async_accept(socket,
                     [this, mesQ](boost::system::error_code ec)
                     {
                         if (!ec)
                         {
                             std::pair<std::string, int> p = std::make_pair(socket.remote_endpoint().address().to_string(), socket.remote_endpoint().port());
                             member_list.push_back(p);
                             auto s = std::make_shared<session>(std::move(socket));
                             s->start(mesQ);
                         }
                         run(mesQ);
                     });
}
