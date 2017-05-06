
/**********************************
 * FILE NAME: Client.cpp
 *
 * Created by Balakrishna Lellapalli on 5/2/17.
 * DESCRIPTION: Definition of all Client related class
 *
 * Copyright © 2017 Balakrishna. All rights reserved.
 **********************************/

#include "Client.hpp"
#include <boost/array.hpp>
using boost::asio::ip::tcp;

Client::Client(boost::asio::io_service& io_service, std::string& connectToAddr, std::string& connectToPort) :
                io_service(io_service), socket(io_service) , resolver(io_service)
{
	resolve(connectToAddr, connectToPort);
}

void Client::write(char* data)
{
	boost::system::error_code err;
	std::string buf(data);
	boost::asio::write(socket, boost::asio::buffer(buf), boost::asio::transfer_all(), err);
}

void Client::resolve(std::string& connectToAddr, std::string& connectToPort)
{
	tcp::resolver::query query(const_cast<char*>(connectToAddr.c_str()), const_cast<char*>(connectToPort.c_str()));
	endpoint_iterator = resolver.resolve(query);
	tcp::resolver::iterator end;
	boost::system::error_code ec = boost::asio::error::host_not_found;
	while(ec && endpoint_iterator != end)
	{
		socket.close();
		socket.connect(*endpoint_iterator++, ec);
	}
}
