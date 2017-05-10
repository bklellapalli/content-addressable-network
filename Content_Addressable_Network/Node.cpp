
/**********************************
 * FILE NAME: Node.cpp
 *
 * Created by Shriram Joshi on 5/1/17
 * DESCRIPTION: Definition of all Node related class
 *
 * Copyright © 2017 Balakrishna. All rights reserved.
 **********************************/
 
#include "Node.hpp"
#include "Message.hpp"
#include "Client.hpp"
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/random.hpp>
#include <cstdlib>
#include <vector>
#include <set>
#include <iostream>

boost::mutex mutex;

Node::Node(boost::asio::io_service& io_service, int port)
{
    sndMsgsQ = new std::queue<std::pair<std::pair<std::string, std::string>, q_elt>>();
    rcvMsgsQ = new std::queue<q_elt>();
    server = new Server{io_service, port, rcvMsgsQ};
    self_address.init(this->getLocalIpAddress(), port);
    long long curtime =  boost::chrono::duration_cast<boost::chrono::milliseconds>(boost::chrono::system_clock::now().time_since_epoch()).count();
    memberList.emplace_back(self_address, ++heartbeat, curtime, self_zone);		// add self entry to member_list entry
}

Node::~Node()
{
	delete sndMsgsQ;
	delete rcvMsgsQ;
	//io_service.close();
	delete server;
}

void Node::recv() 
{
	while(true)
	{
		if(!rcvMsgsQ->empty())
		{
			const char* data = rcvMsgsQ->front().getElement().c_str();
			size_t size = rcvMsgsQ->front().getSize();
			MessageHdr* hdr = (MessageHdr *)(data);
			//LOG_TRACE << "data received " << size;
			std::cout << "-----SIZE" << size <<  std::endl;
			switch(hdr->msgType)
			{
				case HEARTBEAT:
				{
					std::cout << "--HEARTBEAT" << std::endl;
					std::vector<MemberListEntry> memberList;
					this->getMemberList(const_cast<char*>(data));
					std::vector<MemberListEntry>::iterator it_beg = this->memberList.begin();
					std::vector<MemberListEntry>::iterator it_end = this->memberList.end();
					for(; it_beg != it_end; ++it_beg) 
					{
						insertEntry(this->memberList, (*it_beg).getAddress(), (*it_beg).heartbeat, (*it_beg).timestamp);
					}
					break;
				}
				case JOINREQ:
		        // Generate Random coordinate
		        {
		        	std::cout << "--JOINREQ" << std::endl;
		        	short xValue, yValue;
		            memcpy(&xValue, (short*)(data + sizeof(MessageHdr) + (sizeof(char) * 4) + sizeof(short)), sizeof(short));
		            memcpy(&yValue, (short*)(data + sizeof(MessageHdr) + (sizeof(char) * 4) + (sizeof(short) * 2)), sizeof(short));
		            LOG_TRACE << "X : " << xValue << " Y : " << yValue;
		            boost_geometry::point_xy<short> pt;
		            boost::geometry::assign_values(pt, xValue, yValue);
		            LOG_TRACE << "Point (X,Y)" << pt.x() << ", " << pt.y();
		            
		            if(self_zone.isCoordinateInZone(pt))
		            {
		            	std::cout << " Coordinates in zone " << std::endl;
		                char addrA, addrB, addrC, addrD;
		                short port;
		                Zone new_zone = self_zone.splitZone();
		                memcpy(&addrA, (char*)(data + sizeof(MessageHdr)), sizeof(char));
		                memcpy(&addrB, (char*)(data + sizeof(MessageHdr) + sizeof(char) * 1), sizeof(char));
		                memcpy(&addrC, (char*)(data + sizeof(MessageHdr) + sizeof(char) * 2), sizeof(char));
		                memcpy(&addrD, (char*)(data + sizeof(MessageHdr) + sizeof(char) * 3), sizeof(char));
		                memcpy(&port, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4), sizeof(short));
		                Address addr(addrA, addrB, addrC, addrD, port);
		                
		        		long long curtime =  boost::chrono::duration_cast<boost::chrono::milliseconds>(boost::chrono::system_clock::now().time_since_epoch()).count();
		                this->memberList.emplace_back(addr, 1, curtime, new_zone);	//heartbeat set to 1 by default
		                this->memberList.at(0).setZone(self_zone.p1, self_zone.p2, self_zone.p3, self_zone.p4);
		                std::cout << "Address added " << addr.to_string() << std::endl;
		                this->pushMessage(JOINREP, new_zone, addr.to_string(), addr.port_to_string());
		                
		                this->memberList.erase(std::remove_if(this->memberList.begin(), this->memberList.end(),[this](MemberListEntry entry)
		                {
		                    return !this->isNeighbour(entry);
		                }), memberList.end());
		            }
					else
		            {
		            	std::cout << "Coordinates not in zone" << std::endl;
		            	short max_distance = MAX_COORDINATE;
		                std::vector<MemberListEntry>::iterator it_beg = memberList.begin();
		                std::vector<MemberListEntry>::iterator it_end = memberList.end();
		                std::string ipAddress;
		                std::string port;
		                for(; it_beg != it_end; ++it_beg)
		                {
		                    short distance = (*it_beg).findMinDistance(pt);
		                    if(distance < max_distance)
		                    {
		                        ipAddress =  (*it_beg).getAddress().to_string();
		                        port = (*it_beg).getAddress().port_to_string();
		                    }
		                }
		                std::string sData(&data[0], &data[0] + size);
		                q_elt element(sData, size);
		                LOG_TRACE << " forwarding message to " << ipAddress << " on port " << port;
		                auto addr = std::make_pair(ipAddress, port);
		                auto sndMsg = std::make_pair(addr, element);
		                sndMsgsQ->push(sndMsg);
		            }
		        }
		        break;
		            
		        case JOINREP:
		        {
		        	std::cout << "--JOINREP" << std::endl;
		            short p1_x, p1_y, p2_x, p2_y, p3_x, p3_y, p4_x, p4_y;
		            memcpy(&p1_x, data + sizeof(MessageHdr) + (sizeof(char) * 4) + sizeof(short), sizeof(short));
		            memcpy(&p1_y, data + sizeof(MessageHdr) + (sizeof(char) * 4) + sizeof(short) * 2, sizeof(short));
		            memcpy(&p2_x, data + sizeof(MessageHdr) + (sizeof(char) * 4) + sizeof(short) * 3, sizeof(short));
		            memcpy(&p2_y, data + sizeof(MessageHdr) + (sizeof(char) * 4) + sizeof(short) * 4, sizeof(short));
		            memcpy(&p3_x, data + sizeof(MessageHdr) + (sizeof(char) * 4) + sizeof(short) * 5, sizeof(short));
		            memcpy(&p3_y, data + sizeof(MessageHdr) + (sizeof(char) * 4) + sizeof(short) * 6, sizeof(short));
		            memcpy(&p4_x, data + sizeof(MessageHdr) + (sizeof(char) * 4) + sizeof(short) * 7, sizeof(short));
		            memcpy(&p4_y, data + sizeof(MessageHdr) + (sizeof(char) * 4) + sizeof(short) * 8, sizeof(short));
		            std::cout << " p4_y is " << p4_y << std::endl;
		            boost::geometry::assign_values(self_zone.p1, p1_x, p1_y);
		            boost::geometry::assign_values(self_zone.p2, p2_x, p2_y);
		            boost::geometry::assign_values(self_zone.p3, p3_x, p3_y);
		            boost::geometry::assign_values(self_zone.p4, p4_x, p4_y);
		            memberList.at(0).setZone(self_zone.p1, self_zone.p2, self_zone.p3, self_zone.p4);
		            std::cout << "---------ZONE IN RESPONSE--------------" << std::endl;
		            std::cout << " P1 " << self_zone.p1.x() << ", " << self_zone.p1.y() << std::endl;
		            std::cout << " P2 " << self_zone.p2.x() << ", " << self_zone.p2.y() <<std::endl;
		            std::cout << " P3 " << self_zone.p3.x() << ", " << self_zone.p3.y() << std::endl;
		            std::cout << " P4 " << self_zone.p4.x() << ", " << self_zone.p4.y() << std::endl;
		            this->inGroup = true;
		            getMemberList(const_cast<char*>(data));
		            std::vector<MemberListEntry>::iterator it = this->memberList.begin();
		            for(; it != this->memberList.end(); ++it)
		            {
		            	MemberListEntry entry((*it).getAddress(), (*it).heartbeat, (*it).timestamp, (*it).zone);
						std::cout << "Address " << entry.getAddress().to_string() << " port " << entry.getAddress().port_to_string() << std::endl;
						std::cout << "hb " << entry.heartbeat << " tp " << entry.timestamp << std::endl;
						std::cout << " zone " << std::endl;
						std::cout << entry.zone.p1.x() << ", " <<entry.zone.p1.y() << std::endl;
						std::cout << entry.zone.p2.x() << ", " <<entry.zone.p2.y() << std::endl;
						std::cout << entry.zone.p3.x() << ", " <<entry.zone.p3.y() << std::endl;
						std::cout << entry.zone.p4.x() << ", " <<entry.zone.p4.y() << std::endl;
		            }
				}
	            break;
		            
		        case LEAVEREQ:
		        {
		        	std::cout << "--LEAVEREQ" << std::endl;
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
		            this->getMemberList(const_cast<char*>(data));
		        }
		            break;
				
		        case SEARCHFILE: std::cout << "--SEARCHFILE" << std::endl;
		            break;
				
		        case SENDFILE: std::cout << "--Send file" << std::endl;
		            break;
				
		        default: std::cout << "--Default" << std::endl;
		            break;
			}
			std::cout << "--Before pop" << std::endl;
			rcvMsgsQ->pop();
			std::cout << "--After Pop" << std::endl;
		}
	}
}

void Node::sendLoop()
{
	while(true)
	{
		if(!sndMsgsQ->empty())
		{
			//mutex.lock();
			boost::asio::io_service io_service;
			MessageHdr* msg = (MessageHdr*)sndMsgsQ->front().second.getElement().c_str();
			std::cout << "sending msg " << msg->msgType << std::endl;
			std::string address = sndMsgsQ->front().first.first;
		    std::string port = sndMsgsQ->front().first.second;
		    if(((MessageHdr*)msg)->msgType == HEARTBEAT)
			{
				std::vector<MemberListEntry>::iterator it_beg = memberList.begin();
			    std::vector<MemberListEntry>::iterator it_end = memberList.end();
			    for(; it_beg != it_end; ++it_beg)
			    {
			    	if(self_address == (*it_beg).getAddress()) continue;
			        std::string address = (*it_beg).getAddress().to_string();
			        std::string port = (*it_beg).getAddress().port_to_string();
			        Client client(io_service, address, port);
			        client.write(sndMsgsQ->front().second.getElement());
			    }
				
			}
			else
			{
				Client client(io_service, address, port);
			    client.write(sndMsgsQ->front().second.getElement());
			}
			/*if(((MessageHdr*)msg)->msgType == JOINREP)
			{
				std::cout << "Sending Join Reply" <<std::endl;
				Client client(io_service, address, port);
			    client.write(sndMsgsQ->front().second.getElement());
			}
			else if*/
			sndMsgsQ->pop();
			//mutex.unlock();
		}
	}
}

size_t Node::size_of_message(MsgType type)
{
    size_t msgsize = sizeof(MessageHdr) + (4 * sizeof(char) ) + sizeof(short);
    if(type == JOINREQ)
    {
        msgsize += (2 * sizeof(short));	//Requester's X & Y coordinates
    }
    else if(type == HEARTBEAT)
    {
    	msgsize += sizeof(short)/*number of entrie in member_list*/ + ((sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(short) * 8) * memberList.size());
    }
    else if(type == JOINREP)
    {
        msgsize += (sizeof(short) * 8) /*zone coordinates*/ + sizeof(short) /* number of entries in memberlist*/;
        msgsize += (sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(short) * 8) * memberList.size();
    }
    return msgsize;
}

void Node::pushMessage(MsgType type, Zone zone, std::string toAddr, std::string toPort)
{
	size_t size = size_of_message(type);
    
    MessageHdr* hdr = (MessageHdr*) calloc(size, sizeof(char));
    //LOG_TRACE << "before type " << type;
    hdr->msgType = type;
    std::cout << "Message is " << hdr->msgType << std::endl;
    char* ptr = (char*)hdr;
    //LOG_TRACE << "after type " << ((MessageHdr*)ptr)->msgType;
    memcpy((char*)(hdr + 1), &self_address.addrA, sizeof(char));
    memcpy((char*)(hdr + 1) + sizeof(char) * 1, &self_address.addrB, sizeof(char));
    memcpy((char*)(hdr + 1) + sizeof(char) * 2, &self_address.addrC, sizeof(char));
    memcpy((char*)(hdr + 1) + sizeof(char) * 3, &self_address.addrD, sizeof(char));
    memcpy((char*)(hdr + 1) + sizeof(char) * 4, &self_address.port, sizeof(short));
    
    if(type == JOINREQ)
    {
    	//LOG_TRACE << "JOIN REQ CONSTRUCTON";
        memcpy((char*)(hdr + 1) + (sizeof(char) * 4) + sizeof(short), &point.x(), sizeof(short));
        memcpy((char*)(hdr + 1) + (sizeof(char) * 4) + sizeof(short) * 2, &point.y(), sizeof(short));
    }
    if(type == JOINREP)
    {
    	memcpy((char*)(hdr + 1) + (sizeof(char) * 4) + sizeof(short), &zone.p1.x(), sizeof(short));
        memcpy((char*)(hdr + 1) + (sizeof(char) * 4) + sizeof(short) * 2, &zone.p1.y(), sizeof(short));

        memcpy((char*)(hdr + 1) + (sizeof(char) * 4) + sizeof(short) * 3, &zone.p2.x(), sizeof(short));
        memcpy((char*)(hdr + 1) + (sizeof(char) * 4) + sizeof(short) * 4, &zone.p2.y(), sizeof(short));
        
        memcpy((char*)(hdr + 1) + (sizeof(char) * 4) + sizeof(short) * 5, &zone.p3.x(), sizeof(short));
        memcpy((char*)(hdr + 1) + (sizeof(char) * 4) + sizeof(short) * 6, &zone.p3.y(), sizeof(short));

        memcpy((char*)(hdr + 1) + (sizeof(char) * 4) + sizeof(short) * 7, &zone.p4.x(), sizeof(short));
        memcpy((char*)(hdr + 1) + (sizeof(char) * 4) + sizeof(short) * 8, &zone.p4.y(), sizeof(short));
		
		short size_membership_list = memberList.size();
		memcpy((char*)(ptr + sizeof(MessageHdr)) + (sizeof(char) * 4) + sizeof(short) * 9, &size_membership_list, sizeof(short));
        
        fillMemberShipList(ptr);
    }
    std::string data(&ptr[0], &ptr[0] + size);
    q_elt el(data, size);
    auto addr = std::make_pair(toAddr, toPort);
    auto sndMsg = std::make_pair(addr, el);
    sndMsgsQ->push(sndMsg);
    
    
    /*char* mem1 = const_cast<char*>(sndMsgsQ->back().second.getElement().c_str());
    short size1;
    memcpy(&size1, (short*)(mem1 + sizeof(MessageHdr) + (sizeof(char) * 4) + sizeof(short) * 9), sizeof(short));
    std::cout << "_____________________" << size1 << std::endl;
    char* mem = mem1 + sizeof(MessageHdr) + (sizeof(char) * 4) + sizeof(short) * 10;
    for(int i = 0; i < size1; ++i)
    {
		MemberListEntry entry;
		memcpy(&entry.getAddress().addrA, (char*)(mem), sizeof(char));
		memcpy(&entry.getAddress().addrB, (char*)(mem + sizeof(char)), sizeof(char));
		memcpy(&entry.getAddress().addrC, (char*)(mem + sizeof(char) * 2), sizeof(char));
		memcpy(&entry.getAddress().addrD, (char*)(mem + sizeof(char) * 3), sizeof(char));
		memcpy(&entry.getAddress().port, (short*)(mem + sizeof(char) * 4), sizeof(short));
		memcpy(&entry.heartbeat, (long*)(mem + sizeof(char) * 4 + sizeof(short)), sizeof(long));
		memcpy(&entry.timestamp, (long long*)(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long)), sizeof(long long));
		short p1_x, p1_y, p2_x, p2_y, p3_x, p3_y, p4_x, p4_y;
		memcpy(&p1_x, (short*)(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long)), sizeof(short));
		memcpy(&p1_y, (short*)(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(short)), sizeof(short));
		
		memcpy(&p2_x, (short*)(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + (sizeof(short) * 2)), sizeof(short));
		memcpy(&p2_y, (short*)(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + (sizeof(short) * 3)), sizeof(short));
		
		memcpy(&p3_x, (short*)(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + (sizeof(short) * 4)), sizeof(short));
		memcpy(&p3_y, (short*)(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + (sizeof(short) * 5)), sizeof(short));
		
		memcpy(&p4_x, (short*)(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + (sizeof(short) * 6)), sizeof(short));
		memcpy(&p4_y, (short*)(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + (sizeof(short) * 7)), sizeof(short));
		
		boost::geometry::assign_values(entry.zone.p1, p1_x, p1_y);
		boost::geometry::assign_values(entry.zone.p2, p2_x, p2_y);
		boost::geometry::assign_values(entry.zone.p3, p3_x, p3_y);
		boost::geometry::assign_values(entry.zone.p4, p4_x, p4_y);
		
		std::cout << "---------------------------------------------------------------------" << std::endl;
		std::cout << entry.getAddress().to_string() << ", " << entry.getAddress().port_to_string() << ", " << entry.timestamp << std::endl;
		std::cout << entry.zone.p1.x() << ", " << entry.zone.p1.y() << std::endl;
		std::cout << entry.zone.p2.x() << ", " << entry.zone.p2.y() << std::endl;
		std::cout << entry.zone.p3.x() << ", " << entry.zone.p3.y() << std::endl;
		std::cout << entry.zone.p4.x() << ", " << entry.zone.p4.y() << std::endl;
		mem += (sizeof(char) * 4) + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(short) * 8;
	}*/
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
    std::string ipAddress;
    std::string port;
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
                    //displayInfo();
                    //std::cout << "\n******* Connected ********\n" << std::endl;
                    break;
                }
                std::cout << "\nIp Address: ";
                std::cin >> ipAddress;
                std::cout << "\nPort: ";
                std::cin >> port;
                
                pushMessage(JOINREQ, zone, ipAddress, port);
                inGroup = true;
                break;
            case 2:
            {
                if(!inGroup)
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
                        std::string addr = (*it_beg).getAddress().to_string();
                        std::string port = (*it_beg).getAddress().port_to_string();
                        pushMessage(LEAVEREQ, self_zone, addr, port);
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

void Node::getMemberList(char* data) 
{
	short size = 0;
	boost::mutex mt;
	mt.lock();
	memcpy(&size, data + sizeof(MessageHdr) + (sizeof(char) * 4) + sizeof(short) * 9, sizeof(short));
	std::cout << "------->>>>>" << size << std::endl;
 	char* ptr = data + sizeof(MessageHdr) + (sizeof(char) * 4) + sizeof(short) * 10;
 	std::vector<MemberListEntry> temp;
	for(int i = 0; i < size; ++i) 
	{
	
		MemberListEntry entry;
		memcpy(&entry.getAddress().addrA, ptr, sizeof(char));
		memcpy(&entry.getAddress().addrB, ptr + sizeof(char), sizeof(char));
		memcpy(&entry.getAddress().addrC, ptr + sizeof(char) * 2, sizeof(char));
		memcpy(&entry.getAddress().addrD, ptr + sizeof(char) * 3, sizeof(char));
		memcpy(&entry.getAddress().port, ptr + sizeof(char) * 4, sizeof(short));
		memcpy(&entry.heartbeat, ptr + sizeof(char) * 4 + sizeof(short), sizeof(long));
		memcpy(&entry.timestamp, ptr + sizeof(char) * 4 + sizeof(short) + sizeof(long), sizeof(long long));
		
		
		short p1_x, p1_y, p2_x, p2_y, p3_x, p3_y, p4_x, p4_y;
		memcpy(&p1_x, ptr + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long), sizeof(short));
		memcpy(&p1_y, ptr + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(short), sizeof(short));
		memcpy(&p2_x, ptr + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + (sizeof(short) * 2), sizeof(short));
		memcpy(&p2_y, ptr + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + (sizeof(short) * 3), sizeof(short));
		
		memcpy(&p3_x, ptr + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + (sizeof(short) * 4), sizeof(short));
		memcpy(&p3_y, ptr + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + (sizeof(short) * 5), sizeof(short));
		
		memcpy(&p4_x, ptr + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + (sizeof(short) * 6), sizeof(short));
		memcpy(&p4_y, ptr + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + (sizeof(short) * 7), sizeof(short));
		
		boost::geometry::assign_values(entry.zone.p1, p1_x, p1_y);
		boost::geometry::assign_values(entry.zone.p2, p2_x, p2_y);
		boost::geometry::assign_values(entry.zone.p3, p3_x, p3_y);
		boost::geometry::assign_values(entry.zone.p4, p4_x, p4_y);
		
		std::cout << entry.getAddress().to_string() << ", " << entry.getAddress().port_to_string() << ", " << entry.timestamp << std::endl;
		std::cout << entry.zone.p1.x() << ", " << entry.zone.p1.y() << std::endl;
		std::cout << entry.zone.p2.x() << ", " << entry.zone.p2.y() << std::endl;
		std::cout << entry.zone.p3.x() << ", " << entry.zone.p3.y() << std::endl;
		std::cout << entry.zone.p4.x() << ", " << entry.zone.p4.y() << std::endl;
		
		entry.setZone(entry.zone.p1, entry.zone.p2, entry.zone.p3, entry.zone.p4);
		ptr += (4 * sizeof(char)) + sizeof(short) + sizeof(long) + sizeof(long long) + (sizeof(short) * 8);
    	if(isNeighbour(entry))
    	{
    		if(self_address == entry.getAddress())
    		{
    			 std::cout << "-----------------" << entry.getAddress().to_string()  << "----------" << entry.getAddress().port_to_string() << std::endl;	
    		}
    		else 
    		{
    			//this->memberList.emplace_back(entry.getAddress(), entry.heartbeat, entry.timestamp, entry.zone);	//work around
    		}
    	}
    	
	}
	//memberList.insert( memberList.end(), temp.begin(), temp.end() );
	mt.unlock();
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
	
	if(address == (*it_beg).getAddress())
	{
		return;
	}
	if(it_beg == it_end) 
	{
		entry.timestamp = boost::chrono::duration_cast<boost::chrono::milliseconds>
        (boost::chrono::system_clock::now().time_since_epoch()).count();
		memberList.push_back(entry);
	}
	else 
	{
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
	return;
}

void Node::init_mem_protocol(void)
{
	while(true)
	{
		//First check if any removable node
		if(memberList.size() > 1)
		{
			std::vector<MemberListEntry>::iterator it_beg = memberList.begin();
			std::vector<MemberListEntry>::iterator it_end = memberList.end();
			for( /* member list first entry is self node */; it_beg != it_end; ++it_beg) 
			{
				if(self_address == (*it_beg).getAddress()) continue;
				//LOG_TRACE << "check if any node removable";
				long long curtime =  boost::chrono::duration_cast<boost::chrono::milliseconds>(boost::chrono::system_clock::now().time_since_epoch()).count();
				if(!(*it_beg).bDeleted)
				{
					if((curtime - (*it_beg).timestamp) > (TREMOVE + TFAIL)) 
					{
						(*it_beg).bDeleted = true;
					}
				}
			}
		}
		
		//send membership list to neighbours
		if(memberList.size() > 1)	// first entry is self node entry
		{
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
					//No self messaging
					if(!(to == this->self_address))
					{
						size_t msgsize = size_of_message(HEARTBEAT);
						MessageHdr* msg = (MessageHdr *) calloc(msgsize, sizeof(char));
						msg->msgType = HEARTBEAT;
						memcpy((char *)(msg + sizeof(MessageHdr)), &self_address.addrA, sizeof(char));
						memcpy((char *)(msg + sizeof(MessageHdr) + sizeof(char)), &self_address.addrB, sizeof(char));
						memcpy((char *)(msg + sizeof(MessageHdr) + sizeof(char)  * 2), &self_address.addrC, sizeof(char));
						memcpy((char *)(msg + sizeof(MessageHdr) + sizeof(char) * 3), &self_address.addrD, sizeof(char));
						memcpy((char* )(msg + sizeof(MessageHdr) + sizeof(char) * 4), &self_address.port, sizeof(short));
						short numListEntries = (short)memberList.size();
						memcpy((char* )(msg + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short)), &numListEntries, sizeof(short));
						fillMemberShipList((char*)msg);
						auto addressPair = std::make_pair(this->self_address.to_string(), this->self_address.port_to_string());
						const char* buf = (const char*)msg;
						std::string sBuf(&buf[0], &buf[0] + msgsize);
						
						q_elt el(sBuf, msgsize);
						auto sndMsg = std::make_pair(addressPair, el);
						sndMsgsQ->push(sndMsg);
						free (msg);
					}
				}
			}
			sleep(5);
		}
	}
}

void Node::fillMemberShipList(char* msg)
{
	char* mem = msg + sizeof(MessageHdr) + (4 * sizeof(char) ) + sizeof(short) * 10;
	std::vector<MemberListEntry>::iterator it = memberList.begin();
	for(; it != memberList.end(); ++it) 
	{
		if(!(*it).bDeleted)  //If the neighbour is alive
		{
			MemberListEntry entry((*it).getAddress(), (*it).heartbeat, (*it).timestamp, (*it).zone);
			std::cout << "Address " << entry.getAddress().to_string() << " port " << entry.getAddress().port_to_string() << std::endl;
			std::cout << "hb " << entry.heartbeat << " tp " << entry.timestamp << std::endl;
			std::cout << " zone " << std::endl;
			std::cout << entry.zone.p1.x() << ", " <<entry.zone.p1.y() << std::endl;
			std::cout << entry.zone.p2.x() << ", " <<entry.zone.p2.y() << std::endl;
			std::cout << entry.zone.p3.x() << ", " <<entry.zone.p3.y() << std::endl;
			std::cout << entry.zone.p4.x() << ", " <<entry.zone.p4.y() << std::endl;
			memcpy((char *)(mem), &(entry.getAddress().addrA), sizeof(char));
			memcpy((char *)(mem + sizeof(char)), &entry.getAddress().addrB, sizeof(char));
			memcpy((char *)(mem + sizeof(char) * 2), &entry.getAddress().addrC, sizeof(char));
			memcpy((char *)(mem + sizeof(char) * 3), &entry.getAddress().addrD, sizeof(char));
			memcpy((char* )(mem + sizeof(char) * 4), &entry.getAddress().port, sizeof(short));
			memcpy((char* )(mem + sizeof(char) * 4) + sizeof(short), &entry.heartbeat, sizeof(long));
			memcpy((char* )(mem + sizeof(char) * 4) + sizeof(short) + sizeof(long), &entry.timestamp, sizeof(long long));
			
			memcpy((char* )(mem + (sizeof(char) * 4) + sizeof(short) + sizeof(long) + sizeof(long long)), &entry.zone.p1.x(), sizeof(short));
			memcpy((char* )(mem + (sizeof(char) * 4) + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(short)), &entry.zone.p1.y(), sizeof(short));
			memcpy((char* )(mem + (sizeof(char) * 4) + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(short) * 2), &entry.zone.p2.x(), sizeof(short));
			memcpy((char* )(mem + (sizeof(char) * 4) + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(short) * 3), &entry.zone.p2.y(), sizeof(short));
			memcpy((char* )(mem + (sizeof(char) * 4) + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(short) * 4), &entry.zone.p3.x(), sizeof(short));
			memcpy((char* )(mem + (sizeof(char) * 4) + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(short) * 5), &entry.zone.p3.y(), sizeof(short));
			memcpy((char* )(mem + (sizeof(char) * 4) + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(short) * 6), &entry.zone.p4.x(), sizeof(short));
			memcpy((char* )(mem + (sizeof(char) * 4) + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(short) * 7), &entry.zone.p4.y(), sizeof(short));
			
			mem = mem + (sizeof(char) * 4) + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(short) * 8;
		}
	}	
}

short Node::getRandomReceivers(void) 
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
	}
	boost::asio::io_service io_service;
    Node node(io_service, atoi(argv[1]));
    boost::thread t([&io_service](){io_service.run();});
    //boost::thread t1(boost::bind(&Node::init_mem_protocol, &node));
    
	//LOG_TRACE << "Server up & starting";
	boost::thread* t2 = new boost::thread(boost::bind(&Node::recv, boost::ref(node)));
	boost::thread* t4 = new boost::thread(boost::bind(&Node::sendLoop, boost::ref(node)));
	
	std::string response;
	std::cout << "Do you want to join Network[yes/no] : ";
	std::cin >> response;
	if(response.compare("yes") == 0 || response.compare("Yes") == 0 || response.compare("YES") == 0)
	{
		std::string ipAddress;
		std::string port;
		std::cout << "Ip Address of server: ";
        std::cin >> ipAddress;
        std::cout << "Port: ";
        std::cin >> port;
        Zone zone;
        node.pushMessage(JOINREQ, zone, ipAddress, port);
        while(!node.inGroup)
        	continue;
    }
	else
	{
		node.inGroup = true;
	}
    
	//boost::thread t3(boost::bind(&Node::init_mem_protocol, &node));
	//boost::thread t5(boost::bind(&Node::accept_user_input, &node));
    t.join();
    
    //t1.join();
    t2->join();
	//t3.join();
	t4->join();
	//t5.join();
	
	return 0;
}
