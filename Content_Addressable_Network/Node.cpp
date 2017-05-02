//
//  Node.cpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#include "Node.hpp"
#include "Message.hpp"
#include "Client.hpp"
#include <boost/thread.hpp>
#include <cstdlib>

Node::Node(boost::asio::io_service& io_service, int port) :
server {io_service, port, mesQ}
{

}

void Node::getMessages()
{
    char* data = (char*)mesQ->front().elt;
    mesQ->pop();
    MessageHdr* hdr = (MessageHdr *)(data);
    
    switch(hdr->msgType)
    {
            
        case JOINREQ:
            // Generate Rando coordinate
            break;
        
        case LEAVEREQ:
        case SEARCHFILE:
        case SENDFILE:
        case VIEWREQ:
            break;
    }
}

void Node::init_mem_protocol(void)
{
    while(true)
    {
        getMessages();
        sleep(3);
    }
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Node <port>" << std::endl;
    }
    Client client;
    boost::asio::io_service io_service;
    Node node(io_service, atoi(argv[1]));
    boost::thread t1([&io_service](){io_service.run();});
    boost::thread t2(boost::bind(&Node::init_mem_protocol, &node));
    boost::thread t3(boost::bind(&Client::sendMessage, &client));
    t1.join();
    t2.join();
    t3.join();
    
    return 0;
}
