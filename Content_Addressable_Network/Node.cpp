//
//  Member.cpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 4/29/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#include <cstdlib>
#include <vector>
#include <string>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include "Node.hpp"
#include "Message.hpp"
#include "Client.hpp"
#include "Coordinate.hpp"

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
                break;
        	case LEAVEREQ:
                break;
            default:
                break;
		}
	}
}

void Node::sendLoop()
{
	while(!sndMsgsQ->empty())
	{
        boost::asio::io_service io_service;
        char* data = (char*)sndMsgsQ->front().getElement();
        sndMsgsQ->pop();
        MessageHdr* hdr = (MessageHdr *)(data);
        if(hdr->msgType == JOINREQ)
        {
            std::string address = send_to_address.to_string();
            std::string port = send_to_address.port_to_string();
            Client client(io_service, address, port);
            client.write(data);
        }
        else
        {
            std::vector<MemberListEntry> memberList = this->memberList;
            std::vector<MemberListEntry>::iterator it_beg = memberList.begin();
            std::vector<MemberListEntry>::iterator it_end = memberList.end();
            for(; it_beg != it_end; ++it_beg)
            {
                std::string address = (*it_beg).getAddress().to_string();
                std::string port = (*it_beg).getAddress().port_to_string();
                Client client(io_service, address, port);
                client.write(data);
            }
        }
	}
}

size_t Node::size_of_message(MsgTypes type)
{
    size_t msgsize = sizeof(MessageHdr) + (4 * sizeof(char) ) + sizeof(short) + sizeof(char);
    if(type == JOINREQ)
    {
        msgsize += (2 * sizeof(short));
    }
    return msgsize;
}


void Node::pushMessage(MsgTypes type)
{
    MessageHdr* header = (MessageHdr *) malloc(sizeof(MessageHdr) * sizeof(char));
    size_t size = size_of_message(type);
    
    char* ptr = (char *) malloc(size * sizeof(char));
    header->msgType = type;
    
    memcpy((char*)ptr, &header->msgType, sizeof(MessageHdr));
    memcpy((char*)(ptr + sizeof(MessageHdr)), &this->address.addrA, sizeof(char));
    memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 1), &address.addrB, sizeof(char));
    memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 2), &address.addrC, sizeof(char));
    memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 3), &address.addrD, sizeof(char));
    memcpy((short*)(ptr + sizeof(MessageHdr) + sizeof(char) * 4), &address.port, sizeof(short));
    
    if(type == JOINREQ)
    {
        short xValue = coordinates.getX();
        short yValue = coordinates.getY();
        memcpy((short*)(ptr + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short)), &xValue, sizeof(short));
        memcpy((short*)(ptr + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short) * 2), &yValue, sizeof(short));
    }
    q_elt element(ptr, (int)size);
    sndMsgsQ->push(element);
}

void Node::displayInfo()
{
    std::cout << "IP Address: " << this->address.to_string() << std::endl;
    std::cout << "Port: " << this->address.port_to_string() << std::endl;
    std::cout << "Neighbours: " <<  memberList.size() << std::endl;
    std::vector<MemberListEntry>::iterator it_beg = memberList.begin();
    std::vector<MemberListEntry>::iterator it_end = memberList.end();
    int index = 1;
    for(; it_beg != it_end; ++it_beg)
    {
        std::cout << index << ". IP Address: " << (*it_beg).getAddress().to_string() << " ";
        std::cout << "Port: " << (*it_beg).getAddress().port_to_string() << std::endl;
        ++index;
    }
}

void Node::accept_user_input()
{
    char ipAddress[16];
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
                if(inGroup)
                {
                    displayInfo();
                    std::cout << "\n******* Connected ********\n" << std::endl;
                    break;
                }
                std::cout << "\nIp Address: ";
                std::cin >> ipAddress;
                std::cout << "\nPort: ";
                std::cin >> port;
                
                char *token;
                token = std::strtok(ipAddress, ".");
                memcpy(&send_to_address.addrA, token, sizeof(char));
                
                token = strtok(NULL, ".");
                memcpy(&send_to_address.addrB, token, sizeof(char));
                
                token = strtok(NULL, ".");
                memcpy(&send_to_address.addrC, token, sizeof(char));
                
                token = strtok(NULL, ".");
                memcpy(&send_to_address.addrD, token, sizeof(char));
                
                memcpy(&send_to_address.port, &port, sizeof(short));
                pushMessage(JOINREQ);
                inGroup = true;
                break;
            case 2:
                pushMessage(LEAVEREQ);
                inGroup = false;
                break;
            case 3:
                displayInfo();
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
