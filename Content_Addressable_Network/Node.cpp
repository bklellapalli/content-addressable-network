//
//  Node.cpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#include "Node.hpp"
#include "Message.hpp"
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
        case HEARTBEAT:
            std::vector<MemberListEntry> memberList;
            std::vector<MemberListEntry>::iterator it_beg = memberList.begin();
            std::vector<MemberListEntry>::iterator it_end = memberList.end();
            for(; it_beg != it_end; ++it_beg)
            {
                //this->insertEntry(memberNode->memberList, memberNode, (*it_beg).id,
                //    (*it_beg).port, (*it_beg).heartbeat, (*it_beg).timestamp);
            }
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

void Node::performOperation()
{
    //TODO:
    size_t msgsize = sizeof(MessageHdr) + sizeof(std::string) + sizeof(long) + 1;
    MessageHdr* msg = (MessageHdr *) malloc(msgsize * sizeof(char));
    msg->msgType = JOINREQ;
    //memcpy((char *)(msg + 1), &memberNode->addr.addr, sizeof(memberNode->addr.addr));
    //memcpy((char *)(msg + 1) + 1 + sizeof(memberNode->addr.addr), &memberNode->heartbeat, sizeof(long));
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Node <port>" << std::endl;
    }
    boost::asio::io_service io_service;
    Node node(io_service, atoi(argv[1]));
    boost::thread t1([&io_service](){io_service.run();});
    boost::thread t2(boost::bind(&Node::init_mem_protocol, &node));
    boost::thread t3(boost::bind(&Node::performOperation, &node));
    t1.join();
    t2.join();
    t3.join();
    
    return 0;
}
