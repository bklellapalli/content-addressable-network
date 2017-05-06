
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

Server::Server(boost::asio::io_service& io_service, int port, std::queue<q_elt>* mesQ) 
	: endpoint{tcp::v4(), static_cast<unsigned short>(port)}, acc{io_service, endpoint}, socket{io_service}
{
    run(mesQ);
}

Server::~Server()
{
 	socket.close();   
}

void Server::run(std::queue<q_elt>* mesQ)
{
    acc.async_accept(socket,
	 [this, mesQ](boost::system::error_code ec)
	 {
		 if (!ec)
		 {
		     std::make_shared<Server_Session>(std::move(socket))->start(mesQ);
		 }
		 run(mesQ);
	 });
}
