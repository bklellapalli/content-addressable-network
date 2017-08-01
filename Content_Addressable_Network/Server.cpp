
/***************Server.cpp******************
 * FILE NAME: Server.cpp
 *
 * Created by Shriram Joshi on 5/1/17
 * DESCRIPTION: Definition of all Server related class
 *
 * Copyright © 2017 Balakrishna. All rights reserved.
 **********************************/

#include <boost/bind.hpp>
#include "Server.hpp"
#include "Logger.hpp"
#include "SharedQueue.hpp"

Server::Server(boost::asio::io_service& io_service, int port, SharedQueue<q_elt>* mesQ)
	: endpoint{tcp::v4(), static_cast<unsigned short>(port)}, acc{io_service, endpoint}, socket{io_service}
{
	run(mesQ);
}

Server::~Server() { socket.close(); }

void Server::run(SharedQueue<q_elt>* mesQ)
{
    acc.async_accept(socket,
     [this, mesQ](boost::system::error_code ec)
     {
         if (!ec)
         {
             auto s = std::make_shared<Server_Session>(std::move(socket));
             s->start(mesQ);
         }
         run(mesQ);
     });
}
