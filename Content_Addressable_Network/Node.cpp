//
//  Member.cpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 4/29/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#include <cstdlib>
#include <vector>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include "Node.hpp"
#include "Message.hpp"
#include "Client.hpp"

Node::Node(boost::asio::io_service& io_service, int port) :
server {io_service, port, rcvMsgsQ}
{
    sndMsgsQ = new std::queue<q_elt>();
    rcvMsgsQ = new std::queue<q_elt>();
}

Node::~Node()
{
	delete sndMsgsQ;
	delete rcvMsgsQ;
}

void Node::recvLoop() 
{
	if(!rcvMsgsQ->empty())
	{
        char* data = (char*)rcvMsgsQ->front().getElement();
		rcvMsgsQ->pop();
		MessageHdr* hdr = (MessageHdr *)(data);
		switch(hdr->msgType)
		{
			case HEARTBEAT:
			{
				std::vector<MemberListEntry> memberList;
				getMemberList(memberList, data);
				std::vector<MemberListEntry>::iterator it_beg = memberList.begin();
				std::vector<MemberListEntry>::iterator it_end = memberList.end();
				for(; it_beg != it_end; ++it_beg) 
				{
					this->insertEntry(this-> memberList, (*it_beg).getAddress(), (*it_beg).getport(),
                                      (*it_beg).getheartbeat(), (*it_beg).gettimestamp());
				}
				break;
			}
			case JOINREQ:
                // Generate Rando coordinate
                break;
        	case LEAVEREQ:
                break;
		    case SENDMSG:
                break;
		    case VIEWREQ:
                break;
		}
	}
}

void Node::sendLoop()
{
	while(!sndMsgsQ->empty())
	{
		
	}
}

void Node::pushMessage(Address address, MsgTypes type)
{
    MessageHdr* header = (MessageHdr *) malloc(sizeof(MessageHdr) * sizeof(char));
    size_t msgsize = sizeof(MessageHdr) + (4 * sizeof(char) ) + sizeof(address.port) + sizeof(char);
    char* ptr = (char *) malloc(msgsize * sizeof(char));
    
    switch(type)
    {
        case JOINREQ:
            header->msgType = JOINREQ;
            memcpy(&header->msgType, (char*)(ptr), sizeof(MessageHdr));
            memcpy(&address.addrA, (char*)(ptr + sizeof(MessageHdr)), sizeof(char));
            memcpy(&address.addrB, (char*)(ptr + sizeof(MessageHdr) + sizeof(char)), sizeof(char));
            memcpy(&address.addrC, (char*)(ptr + sizeof(MessageHdr) + sizeof(char) + sizeof(char)), sizeof(char));
            memcpy(&address.addrD, (char*)(ptr + sizeof(MessageHdr) + sizeof(char) + sizeof(char) + sizeof(char)), sizeof(char));
            memcpy(&address.port, (short*)(ptr + sizeof(MessageHdr) + sizeof(char) + sizeof(char) + sizeof(char) + sizeof(char)), sizeof(short));
            break;
        case LEAVEREQ: header->msgType = LEAVEREQ;
            memcpy(&header->msgType, (char*)(ptr), sizeof(MessageHdr));
            break;
        case VIEWREQ: //header->msgType = VIEWREQ;
            break;
        case SENDMSG: //header->msgType = SENDMSG;
            break;
        default: break;
    }
    q_elt element(ptr, (int)msgsize);
    sndMsgsQ->push(element);
}

void Node::accept_user_input()
{
    Address address;
    std::string ipAddress = "";
    short port = 0;
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
                pushMessage(address, JOINREQ);
                break;
            case 2:
                pushMessage(address, LEAVEREQ);
                break;
        }
    }
}

void Node::getMemberList(std::vector<MemberListEntry>& memberList, char* data, bool flag) 
{
	char* ptr = data + sizeof(MessageHdr) + sizeof(int);
	//number of entries in data
	int size = 0;
	memcpy(&size, data + sizeof(MessageHdr) + sizeof(4), sizeof(int));
	for(int i = 0; i < size; ++i) {
		MemberListEntry entry;
		memcpy(&entry.getAddress().addrA, (char*)(ptr), sizeof(char));
		memcpy(&entry.getAddress().addrB, (char*)(ptr + sizeof(char)), sizeof(char));
		memcpy(&entry.getAddress().addrC, (char*)(ptr + sizeof(char) + sizeof(char)), sizeof(char));
		memcpy(&entry.getAddress().addrD, (char*)(ptr + sizeof(char) + sizeof(char) + sizeof(char)), sizeof(char));
		memcpy(&entry.port, (short*)(ptr + sizeof(char) + sizeof(char) + sizeof(char) + sizeof(char)), sizeof(short));
		memcpy(&entry.heartbeat, (long*)(ptr + sizeof(char) + sizeof(char) + sizeof(char) + sizeof(char) +
                                         sizeof(short)), sizeof(long));
		memcpy(&entry.timestamp, (long*)(ptr + sizeof(char) + sizeof(char) + sizeof(char) +
                                         sizeof(char) + sizeof(char) + sizeof(short) + sizeof(long)), sizeof(long));
		if(flag)
			this->insertEntry(memberList, entry.getAddress(), entry.getport(), entry.getheartbeat(),
                              entry.gettimestamp(), true);
        else
			this->insertEntry(memberList, entry.getAddress(), entry.getport(), entry.getheartbeat(),
                              entry.gettimestamp(), false);

        ptr += sizeof(char) + sizeof(char) + sizeof(char) + sizeof(char) + sizeof(short) + 2 * sizeof(long);
	}
	return;
}

/**
 * FUNCTION NAME: insertEntry
 *
 * DESCRIPTION: Creates member list entry & inserts into vector
*/
void Node::insertEntry(std::vector<MemberListEntry>& memberList, Address& address,
                       short port, long heartbeat, long timestamp, bool flag)
{
	std::vector<MemberListEntry>::iterator it_beg = memberList.begin();
	std::vector<MemberListEntry>::iterator it_end = memberList.end();
	MemberListEntry entry(address, port, heartbeat, timestamp);
	//If nodes has been deleted then ignore this entry
	//Address addedNode(address);
	if(isNodeRemoved(address, port)) 
	{
		return;
	}
	if(it_beg == it_end) 
	{
		entry.timestamp = boost::chrono::duration_cast<boost::chrono::milliseconds>
        (boost::chrono::system_clock::now().time_since_epoch()).count();
		memberList.push_back(entry);
		//if(flag) {
		//	log->logNodeAdd(&memberNode->addr, &addedNode);
		//}
	}
	else {
		for(; it_beg != it_end; ++it_beg) 
		{
			if(address == (*it_beg).getAddress())
			{
				if(entry.heartbeat > (*it_beg).heartbeat)
				{
					(*it_beg).timestamp = boost::chrono::duration_cast<boost::chrono::milliseconds>
                    (boost::chrono::system_clock::now().time_since_epoch()).count();
					(*it_beg).heartbeat = entry.heartbeat;
					return;
				}
			}
		}
		entry.timestamp = boost::chrono::duration_cast<boost::chrono::milliseconds>
        (boost::chrono::system_clock::now().time_since_epoch()).count();
		memberList.push_back(entry);
		//if(flag) {
		//	log->logNodeAdd(&memberNode->addr, &addedNode);
		//}
	}
	return;
}

bool Node::isNodeRemoved(Address& addr, int port) 
{
	std::vector<MemberListEntry>:: iterator it_beg = this->memberList.begin();
	std::vector<MemberListEntry>:: iterator it_end = this->memberList.end();
	for(; it_beg != it_end; ++it_beg) 
	{
		MemberListEntry entry = *it_beg;
		if(addr == entry.getAddress() && port == entry.port)
		{
			return true;
		}
	}
	return false;
}

void Node::init_mem_protocol(void)
{
	while(true)
	{
		recvLoop();
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
	
    boost::thread t1([&io_service](){io_service.run();});
	boost::thread t2(boost::bind(&Node::init_mem_protocol, &node));
	boost::thread t3(boost::bind(&Node::sendLoop, &node));
    boost::thread t4(boost::bind(&Node::accept_user_input, &node));
    
    t1.join();
    t2.join();
	t3.join();
    t4.join();
	
	return 0;
}
