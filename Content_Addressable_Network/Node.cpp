
/**********************************
 * FILE NAME: Node.cpp
 *
 * Created by Shriram Joshi on 5/1/17
 * DESCRIPTION: Definition of all Node related class
 *
 * Copyright © 2017 Balakrishna. All rights reserved.
 **********************************/

#include <cstdlib>
#include <vector>
#include <set>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/random.hpp>

#include "Node.hpp"
#include "Message.hpp"
#include "Client.hpp"

Node::Node(boost::asio::io_service& io_service, int port)
{
    sndMsgsQ = new std::queue<q_elt>();
    rcvMsgsQ = new std::queue<q_elt>();
    server = new Server{io_service, port, rcvMsgsQ};
}

Node::~Node()
{
	delete sndMsgsQ;
	delete rcvMsgsQ;
	delete server;
}

void Node::recv() 
{
	if(!rcvMsgsQ->empty())
	{
		char* data = (char*)rcvMsgsQ->front().getElement();
		size_t size = rcvMsgsQ->front().getSize();
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
					insertEntry(this->memberList, (*it_beg).getAddress(), (*it_beg).heartbeat, (*it_beg).timestamp);
				}
				break;
			}
			case JOINREQ:
            // Generate Random coordinate
            {
                short xValue, yValue;
                memcpy(&xValue, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short)), sizeof(short));
                memcpy(&yValue, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short) * 2), sizeof(short));
                
                boost_geometry::point_xy<short> pt;
                boost::geometry::assign_values(pt, xValue, yValue);
                
                if(self_zone.isCoordinateInZone(pt))
                {
                    char addrA, addrB, addrC, addrD;
                    short port;
                    
                    Zone new_zone = self_zone.splitZone();
                    memcpy(&addrA, (char*)(data + sizeof(MessageHdr)), sizeof(char));
                    memcpy(&addrB, (char*)(data + sizeof(MessageHdr) + sizeof(char) * 1), sizeof(char));
                    memcpy(&addrC, (char*)(data + sizeof(MessageHdr) + sizeof(char) * 2), sizeof(char));
                    memcpy(&addrD, (char*)(data + sizeof(MessageHdr) + sizeof(char) * 3), sizeof(char));
                    memcpy(&port, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4), sizeof(short));
                    Address addr(addrA, addrB, addrC, addrD, port);
            
                    // Send join reply message
                    send_to_address =  addr;
                    pushMessage(JOINREP, new_zone);
                    
                    MemberListEntry entry(addr, new_zone);
                    memberList.push_back(entry);
                    memberList.erase(std::remove_if(memberList.begin(), memberList.end(),[this](MemberListEntry entry)
                    {
                        return !isNeighbour(entry);
                    }), memberList.end());
				}
				else
                {
                    short max_distance = MAX_COORDINATE;
                    std::vector<MemberListEntry>::iterator it_beg = memberList.begin();
                    std::vector<MemberListEntry>::iterator it_end = memberList.end();
                    for(; it_beg != it_end; ++it_beg)
                    {
                        short distance = (*it_beg).findMinDistance(pt);
                        if(distance < max_distance)
                        {
                            send_to_address =  (*it_beg).getAddress();
                        }
                    }
                    q_elt element(data, size);
                    sndMsgsQ->push(element);
                }
            }
            break;
                
            case JOINREP:
            {
                short p1_x, p1_y, p2_x, p2_y, p3_x, p3_y, p4_x, p4_y;
                memcpy(&p1_x, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 +
                                                  sizeof(short)), sizeof(short));
                memcpy(&p1_y, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 +
                                              sizeof(short) * 2), sizeof(short));
                
                memcpy(&p2_x, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 +
                                                   sizeof(short) * 3), sizeof(short));
                memcpy(&p2_y, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 +
                                                   sizeof(short) * 4), sizeof(short));
                
                memcpy(&p3_x, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 +
                                                   sizeof(short) * 5), sizeof(short));
                memcpy(&p3_y, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 +
                                                   sizeof(short) * 6), sizeof(short));
                
                memcpy(&p4_x, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 +
                                                   sizeof(short) * 7), sizeof(short));
                memcpy(&p4_y, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 +
                                                   sizeof(short) * 8), sizeof(short));
                
                boost::geometry::assign_values(self_zone.p1, p1_x, p1_y);
                boost::geometry::assign_values(self_zone.p2, p2_x, p2_y);
                boost::geometry::assign_values(self_zone.p3, p3_x, p3_y);
                boost::geometry::assign_values(self_zone.p4, p4_x, p4_y);
                
                getMemberList(memberList, data);
            }
                break;
                
            case LEAVEREQ:
            {
                short p1_x, p1_y, p2_x, p2_y, p3_x, p3_y, p4_x, p4_y;
                memcpy(&p1_x, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 +
                                       sizeof(short)), sizeof(short));
                memcpy(&p1_y, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 +
                                       sizeof(short) * 2), sizeof(short));
                
                memcpy(&p2_x, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 +
                                       sizeof(short) * 3), sizeof(short));
                memcpy(&p2_y, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 +
                                       sizeof(short) * 4), sizeof(short));
                
                memcpy(&p3_x, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 +
                                       sizeof(short) * 5), sizeof(short));
                memcpy(&p3_y, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 +
                                       sizeof(short) * 6), sizeof(short));
                
                memcpy(&p4_x, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 +
                                       sizeof(short) * 7), sizeof(short));
                memcpy(&p4_y, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 +
                                       sizeof(short) * 8), sizeof(short));
                
                Zone newZone;
                boost::geometry::assign_values(newZone.p1, p1_x, p1_y);
                boost::geometry::assign_values(newZone.p2, p2_x, p2_y);
                boost::geometry::assign_values(newZone.p3, p3_x, p3_y);
                boost::geometry::assign_values(newZone.p4, p4_x, p4_y);
                
                //Merge Zone
                self_zone.mergeZone(newZone);
                
                // update membership list
                getMemberList(memberList, data);
            }
                break;
		    
            case SEARCHFILE:
                break;
		    
            case SENDFILE:
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
	    
        if(hdr->msgType == JOINREQ || hdr->msgType == JOINREP)
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

size_t Node::size_of_message(MsgType type)
{
    size_t msgsize = sizeof(MessageHdr) + (4 * sizeof(char) ) + sizeof(short) + sizeof(char);
    if(type == JOINREQ)
    {
        msgsize += (2 * sizeof(short));
    }
    if(type == HEARTBEAT)
    {
    	msgsize += sizeof(short)/*number of entrie in member_list*/ + sizeof(MemberListEntry) * memberList.size();
    }
    if(type == JOINREP)
    {
        msgsize += sizeof(short) * 8;
        msgsize += sizeof(short) + sizeof(MemberListEntry) * memberList.size();
    }
    return msgsize;
}

void Node::pushMessage(MsgType type, Zone zone)
{
    MessageHdr* header = (MessageHdr *) malloc(sizeof(MessageHdr) * sizeof(char));
    size_t size = size_of_message(type);
    
    char* ptr = (char *) malloc(size * sizeof(char));
    header->msgType = type;
    
    memcpy((char*)ptr, &header->msgType, sizeof(MessageHdr));
    memcpy((char*)(ptr + sizeof(MessageHdr)), &this->self_address.addrA, sizeof(char));
    memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 1), &self_address.addrB, sizeof(char));
    memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 2), &self_address.addrC, sizeof(char));
    memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 3), &self_address.addrD, sizeof(char));
    memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 4), &self_address.port, sizeof(short));
    
    if(type == JOINREQ)
    {
        memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short)), &point.x(), sizeof(short));
        memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short) * 2), &point.y(), sizeof(short));
    }
    if(type == JOINREP || type == LEAVEREQ)
    {
        memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short)), &zone.p1.x(), sizeof(short));
        memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short) * 2), &zone.p1.y(), sizeof(short));

        memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short) * 3), &zone.p2.x(), sizeof(short));
        memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short) * 4), &zone.p2.y(), sizeof(short));
        
        memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short) * 5), &zone.p3.x(), sizeof(short));
        memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short) * 6), &zone.p3.y(), sizeof(short));

        memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short) * 7), &zone.p4.x(), sizeof(short));
        memcpy((char*)(ptr + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short) * 8), &zone.p4.y(), sizeof(short));

        short size_membership_list = memberList.size();
        memcpy((char*)(ptr +sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short) * 9), &size_membership_list, sizeof(short));
        fillMemberShipList((char *)ptr);
    }
    q_elt element(ptr, (int)size);
    sndMsgsQ->push(element);
}

void Node::displayInfo()
{
    std::cout << "IP Address: " << this->self_address.to_string() << std::endl;
    std::cout << "Port: " << this->self_address.port_to_string() << std::endl;
    std::cout << "Neighbours: " <<  memberList.size() << std::endl;
    std::vector<MemberListEntry>::iterator it_beg = memberList.begin();
    std::vector<MemberListEntry>::iterator it_end = memberList.end();
    for(int index = 0; it_beg != it_end; ++it_beg, ++index)
    {
        std::cout << index << ". IP Address: " << (*it_beg).getAddress().to_string() << " ";
        std::cout << "Port: " << (*it_beg).getAddress().port_to_string() << std::endl;
    }
}

void Node::accept_user_input()
{
    Zone zone;
    char ipAddress[16];
    short port = 0;
    short option = 0;
    
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
                pushMessage(JOINREQ, zone);
                inGroup = true;
                break;
            case 2:
            {
                if(inGroup)
                {
                    std::cout << "\n******* Disconnected ********\n" << std::endl;
                    break;
                }
                std::vector<MemberListEntry>::iterator it_beg = memberList.begin();
                std::vector<MemberListEntry>::iterator it_end = memberList.end();
                for(; it_beg != it_end; ++it_beg)
                {
                    if(self_zone.canMergeZone((*it_beg).getZone()))
                    {
                        send_to_address = (*it_beg).getAddress();
                        pushMessage(LEAVEREQ, self_zone);
                        inGroup = false;
                        break;
                    }
                }
                std::cout << "\n******* Disconnected ********\n" << std::endl;
            }
                break;
            case 3:
                displayInfo();
                break;
        }
    }
}

void Node::getMemberList(std::vector<MemberListEntry>& memberList, char* data, bool flag) 
{
	char* ptr = data + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short) + sizeof(short) * 8;
	//number of entries in data
	short size = 0;
	memcpy(&size, (short *)ptr , sizeof(short));
    ptr += sizeof(short);
	for(int i = 0; i < size; ++i) {
		MemberListEntry entry;
		memcpy(&entry.getAddress().addrA, (char*)(ptr), sizeof(char));
		memcpy(&entry.getAddress().addrB, (char*)(ptr + sizeof(char)), sizeof(char));
		memcpy(&entry.getAddress().addrC, (char*)(ptr + sizeof(char) * 2), sizeof(char));
		memcpy(&entry.getAddress().addrD, (char*)(ptr + sizeof(char) * 3), sizeof(char));
		memcpy(&entry.getAddress().port, (short*)(ptr + sizeof(char) * 4), sizeof(short));
		memcpy(&entry.heartbeat, (long*)(ptr + sizeof(char) * 4 + sizeof(short)), sizeof(long));
		memcpy(&entry.timestamp, (long*)(ptr + sizeof(char) * 4 + sizeof(short) + sizeof(long)), sizeof(long));
        this->insertEntry(memberList, entry.getAddress(), entry.getheartbeat(), entry.gettimestamp());
        ptr += sizeof(char) + sizeof(char) + sizeof(char) + sizeof(char) + sizeof(short) + 2 * sizeof(long);
	}
	return;
}

/**
 * FUNCTION NAME: insertEntry
 *
 * DESCRIPTION: Creates member list entry & inserts into vector
*/
void Node::insertEntry(std::vector<MemberListEntry>& memberList, Address& address, long heartbeat, long long timestamp) {
	
    std::vector<MemberListEntry>::iterator it_beg = memberList.begin();
	std::vector<MemberListEntry>::iterator it_end = memberList.end();
	MemberListEntry entry(address, heartbeat, timestamp);
	
    if(address == (*it_beg).getAddress()) return;
	if(it_beg == it_end) 
	{
		entry.timestamp = boost::chrono::duration_cast<boost::chrono::milliseconds>
        (boost::chrono::system_clock::now().time_since_epoch()).count();
		memberList.push_back(entry);
	}
	else {
		for(; it_beg != it_end; ++it_beg) 
		{
			if(entry.getAddress() == (*it_beg).getAddress())
			{
				if((*it_beg).bDeleted == true)
				{
					//if neighbour was previously deleted
					if((*it_beg).timestamp < entry.timestamp)
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
				else if(entry.heartbeat > (*it_beg).heartbeat)
				{
					(*it_beg).timestamp = boost::chrono::duration_cast<boost::chrono::milliseconds>
                    (boost::chrono::system_clock::now().time_since_epoch()).count();
					(*it_beg).heartbeat = entry.heartbeat;
					return;
				}
			}
		}
		entry.timestamp = boost::chrono::duration_cast<boost::chrono::milliseconds>(boost::chrono::system_clock::now().time_since_epoch()).count();
		memberList.push_back(entry);
	}
}

void Node::init_mem_protocol(void)
{
	while(true)
	{
		//First receive message
		recv();
		
		//First check if any removable node
		std::vector<MemberListEntry>::iterator it_beg = memberList.begin();
		std::vector<MemberListEntry>::iterator it_end = memberList.end();
		for(; it_beg != it_end; ++it_beg) 
		{
			MemberListEntry entry = *it_beg++; //first entry is self node in memberlist
			long long curtime =  boost::chrono::duration_cast<boost::chrono::milliseconds>
                                    (boost::chrono::system_clock::now().time_since_epoch()).count();
			if((curtime - entry.timestamp) > (TREMOVE + TFAIL)) 
			{
				(*it_beg).bDeleted = true;
			}
		}
		
		//send membership list to neighbours
		std::set<int> vec_receivers;
		std::set<int>::iterator it;
	  	std::pair<std::set<int>::iterator,bool> ret;
		short rand_receivers = getRandomReceivers();
		//Increment self heat beat by before sending to neighbours
		memberList.at(0).heartbeat++;
		for(int i = 0; i < rand_receivers; ++i) 
		{
			int rand_rcvr = getRandomReceivers() % memberList.size();
			ret = vec_receivers.insert(rand_rcvr);
			//No receiver gets heartbeat twice
			if(ret.second != false) 
			{
				MemberListEntry receiver = memberList.at(rand_rcvr);
				Address to(receiver.getAddress());
				//No self messaging
				if(!(to == this->self_address))
				{
					size_t msgsize = size_of_message(HEARTBEAT);
					MessageHdr* msg = (MessageHdr *) calloc(msgsize, sizeof(char));
					msg->msgType = HEARTBEAT;
					memcpy((char *)(msg + 1), &self_address.addrA, sizeof(char));
					memcpy((char *)(msg + 1 + sizeof(char)), &self_address.addrA, sizeof(char));
					memcpy((char *)(msg + 1 + sizeof(char)  * 2), &self_address.addrA, sizeof(char));
					memcpy((char *)(msg + 1 + sizeof(char) * 3), &self_address.addrA, sizeof(char));
					memcpy((char* )(msg + 1) + sizeof(char) * 4, &self_address.port, sizeof(short));
					short numListEntries = (short)memberList.size();
					memcpy((char* )(msg + 1) + sizeof(char) * 4 + sizeof(short), &numListEntries, sizeof(short));
					fillMemberShipList((char*)msg);
				}
			}
		}
		//sleep(3);
	}
}

void Node::fillMemberShipList(char* msg)
{
	char* mem = (char*)(msg + sizeof(MessageHdr) + (4 * sizeof(char) ) + sizeof(short) + sizeof(char) + sizeof(short));
	std::vector<MemberListEntry>::iterator it = memberList.begin();
	for(; it != memberList.end(); ++it) {
		
		if(!(*it).bDeleted)  //If the neighbour is alive
		{
			MemberListEntry entry((*it).getAddress(), (*it).heartbeat, (*it).timestamp, (*it).zone);
			memcpy((char *)(mem), &entry.getAddress().addrA, sizeof(char));
			memcpy((char *)(mem + sizeof(char)), &entry.getAddress().addrB, sizeof(char));
			memcpy((char *)(mem + sizeof(char) * 2), &entry.getAddress().addrC, sizeof(char));
			memcpy((char *)(mem + sizeof(char) * 3), &entry.getAddress().addrD, sizeof(char));
			memcpy((char* )(mem + sizeof(char) * 4), &entry.getAddress().port, sizeof(short));
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short)), &entry.heartbeat, sizeof(long));
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long)), &entry.timestamp, sizeof(long long));
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long)),
                   &entry.zone.p1.x(), sizeof(short));
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) +
                            sizeof(short)), &entry.zone.p1.y(), sizeof(int));
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) +
                            sizeof(short) * 2), &entry.zone.p2.x(), sizeof(short));
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) +
                            sizeof(short) * 3), &entry.zone.p2.y(), sizeof(short));
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) +
                            sizeof(short) * 4), &entry.zone.p3.x(), sizeof(short));
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) +
                            sizeof(short) * 5), &entry.zone.p3.y(), sizeof(short));
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) +
                            sizeof(short) * 6), &entry.zone.p4.x(), sizeof(short));
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) +
                            sizeof(short) * 7), &entry.zone.p4.y(), sizeof(short));
			
			mem += sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(short) * 8;
		}
	}
}

int Node::getRandomReceivers(void)
{
	int max = memberList.size() - 1;
	std::time_t now = std::time(0);
    boost::random::mt19937 gen{static_cast<std::uint32_t>(now)};
    boost::random::uniform_int_distribution<> dist{0, max};
	return dist(gen);
}

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		std::cerr << "Node <port>" << std::endl;
        return -1;
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
