//
//  Client.cpp
//  Content_Addressable_Network
//
//  Created by Balakrishna Lellapalli on 5/2/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#include "Client.hpp"
#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::tcp;

/*MessageHdr* Client::constructMessage(std::string ipAddress, long port, MsgTypes type)
{
    size_t msgsize = sizeof(MessageHdr) + sizeof(ipAddress) + sizeof(port) + 1;
    MessageHdr* msg = (MessageHdr *) malloc(msgsize * sizeof(char));
    
    switch(type)
    {
        case JOINREQ:
            msg->msgType = JOINREQ;
            memcpy((char *)(msg + sizeof(MessageHdr)) + 1, &ipAddress, sizeof(ipAddress));
            memcpy((char *)(msg + sizeof(MessageHdr)) + sizeof(ipAddress) + 1, &port, sizeof(int));
            break;
        
        case LEAVEREQ:
        case SEARCHFILE:
        case SENDFILE:
        case VIEWREQ:
            break;
    }
    return msg;
}

void Client::openSocket(std::string ipAddress, long port, MessageHdr* msg)
{
    try
    {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(ipAddress, std::to_string(port));
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::resolver::iterator end;
        tcp::socket socket(io_service);
        boost::system::error_code ec = boost::asio::error::host_not_found;
        while(ec && endpoint_iterator != end)
        {
            socket.close();
            socket.connect(*endpoint_iterator++, ec);
        }
        if(ec)
        {
            throw boost::system::system_error(ec);
        }
        boost::system::error_code err;
        boost::asio::write(socket, boost::asio::buffer(msg, sizeof(msg)),
                          boost::asio::transfer_all(), err);
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Client::sendMessage()
{
    std::string ipAddress = "";
    long port = 0;
    int option = 0;
    
    while(true)
    {
        std::cout << "____________MENU____________" << std::endl;
        std::cout << "1. Join Network" << std::endl;
        std::cout << "2. Leave Network" << std::endl;
        std::cout << "3. View Network" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> option;
        
        switch(option)
        {
            case 1:
                std::cout << "\nIp Address: ";
                std::cin >> ipAddress;
                std::cout << "\nPort: ";
                std::cin >> port;
                MessageHdr* msg = constructMessage(ipAddress, port, JOINREQ);
                openSocket(ipAddress, port, msg);
                break;
        }
    }
}*/

