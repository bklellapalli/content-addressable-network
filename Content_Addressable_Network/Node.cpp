//
//  Node.cpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#include "Node.hpp"
#include <boost/thread.hpp>
#include <cstdlib>

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
    
    boost::asio::io_service io_service;
    Node node(io_service, atoi(argv[1]));
    boost::thread t([&io_service](){io_service.run();});
    boost::thread t1(boost::bind(&Node::init_mem_protocol, &node));
    t1.join();
    t.join();
    //t1.join();
    return 0;
}

/**
 enum MsgTypes{
 JOINREQ,
 JOINREP,
 HEARTBEAT,
 DUMMYLASTMSGTYPE
 };
 
 * STRUCT NAME: MessageHdr
 *
 * DESCRIPTION: Header and content of a message
 typedef struct MessageHdr {
	enum MsgTypes msgType;
 }MessageHdr;
 size_t msgsize = sizeof(MessageHdr) + sizeof(joinaddr->addr) + sizeof(long) + 1;
 msg = (MessageHdr *) malloc(msgsize * sizeof(char));
 // create JOINREQ message: format of data is {struct Address myaddr}
 msg->msgType = JOINREQ;
 memcpy((char *)(msg + 1), &memberNode->addr.addr, sizeof(memberNode->addr.addr));
 memcpy((char *)(msg + 1) + 1 + sizeof(memberNode->addr.addr), &memberNode->heartbeat, sizeof(long));
 */
