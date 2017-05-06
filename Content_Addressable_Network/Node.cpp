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
	//io_service.close();
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
					this->insertEntry(this->memberList, (*it_beg).getAddress(), (*it_beg).port, (*it_beg).heartbeat, (*it_beg).timestamp);
				}
				break;
			}
			case JOINREQ:
            // Generate Random coordinate
            {
                short xValue, yValue;
                memcpy(&xValue, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short)), sizeof(short));
                memcpy(&yValue, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4 + sizeof(short) * 2), sizeof(short));
                
                boost::geometry::model::d2::point_xy<int> pt;
                boost::geometry::assign_values(pt, xValue, yValue);
                
                if(current_zone.isCoordinateInZone(pt))
                {
                    char addrA, addrB, addrC, addrD;
                    short port;
                    
                    Zone new_zone = current_zone.splitZone();
                    memcpy(&addrA, (char*)(data + sizeof(MessageHdr)), sizeof(char));
                    memcpy(&addrB, (char*)(data + sizeof(MessageHdr) + sizeof(char) * 1), sizeof(char));
                    memcpy(&addrC, (char*)(data + sizeof(MessageHdr) + sizeof(char) * 2), sizeof(char));
                    memcpy(&addrD, (char*)(data + sizeof(MessageHdr) + sizeof(char) * 3), sizeof(char));
                    memcpy(&port, (short*)(data + sizeof(MessageHdr) + sizeof(char) * 4), sizeof(short));
                    Address addr(addrA, addrB, addrC, addrD, port);
            
                    // TODO: Send membership list entry in msg body
                    
                    MemberListEntry entry(addr, port, new_zone);
                    memberList.push_back(entry);
                    memberList.erase(std::remove_if(memberList.begin(), memberList.end(),[this](MemberListEntry entry)
                    {
                        return !isNeighbour(entry);
                    }), memberList.end());
				}
				else
                {
                    short max_distance = 100;
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
        	case LEAVEREQ: break;
		    case SEARCHFILE: break;
		    case SENDFILE: break;
		    case VIEWREQ: break;
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

size_t Node::size_of_message(MsgType type)
{
    size_t msgsize = sizeof(MessageHdr) + (4 * sizeof(char) ) + sizeof(short) + sizeof(char);
    if(type == JOINREQ)
    {
        msgsize += (2 * sizeof(short));
    }
    if(type == HEARTBEAT)
    {
    	msgsize += sizeof(int)/*number of entrie in member_list*/ + sizeof(MemberListEntry) * memberList.size();
    }
    return msgsize;
}

/*int	Node::sendHeartBeat(Address* to, char* msg, int size) {
	char* ptr = msg + sizeof(MessageHdr) + sizeof(memberNode->addr.addr) + sizeof(int);
	vector<MemberListEntry>::iterator it = memberNode->memberList.begin();
	for(; it != memberNode->memberList.end(); ++it) {
		
		if((*it).id > 0) {
			MemberListEntry entry((*it).id, (*it).port, (*it).heartbeat, (*it).timestamp);
			memcpy((char* )(ptr), &entry.id, sizeof(int));														//NODE id
			memcpy((char* )(ptr + sizeof(int)), &entry.port, sizeof(short));									//NODE port
			memcpy((char* )(ptr + sizeof(int) + sizeof(short)), &entry.heartbeat, sizeof(long));				//NODE heartbeat
			memcpy((char* )(ptr + sizeof(int) + sizeof(short) + sizeof(long)), &entry.timestamp, sizeof(long));	//NODE local-timestamp
			ptr += sizeof(int) + sizeof(short) + 2 * sizeof(long);
		}
	}
	return emulNet->ENsend(&memberNode->addr, to, msg, size);
}
*/

void Node::pushMessage(MsgType type)
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
        short xValue = point.x();
		short yValue = point.y();
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
        this->insertEntry(memberList, entry.getAddress(), entry.getport(), entry.getheartbeat(), entry.gettimestamp());
		/*if(flag)
			
        else
			this->insertEntry(memberList, entry.getAddress(), entry.getport(), entry.getheartbeat(), entry.gettimestamp(), false);*/

        ptr += sizeof(char) + sizeof(char) + sizeof(char) + sizeof(char) + sizeof(short) + 2 * sizeof(long);
	}
	return;
}

/**
 * FUNCTION NAME: insertEntry
 *
 * DESCRIPTION: Creates member list entry & inserts into vector
*/
void Node::insertEntry(std::vector<MemberListEntry>& memberList, Address& address, short port, long heartbeat, long long timestamp) {
	std::vector<MemberListEntry>::iterator it_beg = memberList.begin();
	std::vector<MemberListEntry>::iterator it_end = memberList.end();
	MemberListEntry entry(address, port, heartbeat, timestamp);
	
	if(address == (*it_beg).getAddress())
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
		//if(flag) {
		//	log->logNodeAdd(&memberNode->addr, &addedNode);
		//}
	}
	return;
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
			long long curtime =  boost::chrono::duration_cast<boost::chrono::milliseconds>(boost::chrono::system_clock::now().time_since_epoch()).count();
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
				if(!(to == this->address))
				{
					size_t msgsize = size_of_message(HEARTBEAT);
					MessageHdr* msg = (MessageHdr *) calloc(msgsize, sizeof(char));
					msg->msgType = HEARTBEAT;
					memcpy((char *)(msg + 1), &address.addrA, sizeof(char));
					memcpy((char *)(msg + 1 + sizeof(char)), &address.addrA, sizeof(char));
					memcpy((char *)(msg + 1 + sizeof(char)  * 2), &address.addrA, sizeof(char));
					memcpy((char *)(msg + 1 + sizeof(char) * 3), &address.addrA, sizeof(char));
					memcpy((char* )(msg + 1) + sizeof(char) * 4, &address.port, sizeof(short));
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
			MemberListEntry entry((*it).getAddress(), (*it).port, (*it).heartbeat, (*it).timestamp, (*it).current_zone);
			memcpy((char *)(mem), &address.addrA, sizeof(char));
			memcpy((char *)(mem + sizeof(char)), &address.addrB, sizeof(char));
			memcpy((char *)(mem + sizeof(char) * 2), &address.addrC, sizeof(char));
			memcpy((char *)(mem + sizeof(char) * 3), &address.addrD, sizeof(char));
			memcpy((char* )(mem + sizeof(char) * 4), &address.port, sizeof(short));
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short)), &entry.heartbeat, sizeof(long));																//NODE heartbeat
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long)), &entry.timestamp, sizeof(long long));											//NODE local-timestamp
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long)), &entry.current_zone.p1.x(), sizeof(int));					//NODE p1.x
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(int)), &entry.current_zone.p1.y(), sizeof(int));		//NODE p1.y
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(int) * 2), &entry.current_zone.p2.x(), sizeof(int));	//NODE p2.x
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(int) * 3), &entry.current_zone.p2.y(), sizeof(int));	//NODE p2.y
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(int) * 4), &entry.current_zone.p3.x(), sizeof(int));	//NODE p3.x
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(int) * 5), &entry.current_zone.p3.y(), sizeof(int));	//NODE p3.y
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(int) * 6), &entry.current_zone.p4.x(), sizeof(int));	//NODE p4.x
			memcpy((char* )(mem + sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(int) * 7), &entry.current_zone.p4.y(), sizeof(int));	//NODE p4.y
			
			mem += sizeof(char) * 4 + sizeof(short) + sizeof(long) + sizeof(long long) + sizeof(int) * 8;
		}
	}
}

short Node::getRandomReceivers(void) {
	int max = memberList.size() - 1;
	std::time_t now_x = std::time(0);
    boost::random::mt19937 gen_x{static_cast<std::uint32_t>(now_x)};
    boost::random::uniform_int_distribution<> dist_x{0, max};
	return dist_x(gen_x);
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
