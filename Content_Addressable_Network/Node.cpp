#include "Node.hpp"
#include "Message.hpp"
#include "Client.hpp"
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <cstdlib>
#include <vector>

Node::~Node()
{
	delete sndMsgsQ;
	delete rcvMsgsQ;
}

void Node::recvLoop() 
{
	if(!rcvMsgsQ->empty())
	{
		char* data = (char*)rcvMsgsQ->front().elt;
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
					this->insertEntry(this->memberList, (*it_beg).address, (*it_beg).port, (*it_beg).heartbeat, (*it_beg).timestamp);
				}
				break;
			}
			case JOINREQ:
            // Generate Rando coordinate
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
		memcpy(&entry.address.addrA, (char*)(ptr), sizeof(char));
		memcpy(&entry.address.addrB, (char*)(ptr + sizeof(char)), sizeof(char));
		memcpy(&entry.address.addrC, (char*)(ptr + sizeof(char) + sizeof(char)), sizeof(char));
		memcpy(&entry.address.addrD, (char*)(ptr + sizeof(char) + sizeof(char) + sizeof(char)), sizeof(char));
		memcpy(&entry.port, (short*)(ptr + sizeof(char) + sizeof(char) + sizeof(char) + sizeof(char)), sizeof(short));
		memcpy(&entry.heartbeat, (long*)(ptr + sizeof(char) + sizeof(char) + sizeof(char) + sizeof(char) + sizeof(short)), sizeof(long));
		memcpy(&entry.timestamp, (long*)(ptr + sizeof(char) + sizeof(char) + sizeof(char) + sizeof(char) + sizeof(char) + sizeof(short) + sizeof(long)), sizeof(long));
		if(flag)
			this->insertEntry(memberList, entry.address, entry.port, entry.heartbeat, entry.timestamp, true);
		else 
			this->insertEntry(memberList, entry.address, entry.port, entry.heartbeat, entry.timestamp, false);
		ptr += sizeof(char) + sizeof(char) + sizeof(char) + sizeof(char) + sizeof(short) + 2 * sizeof(long);
	}
	return;
}

/**
 * FUNCTION NAME: insertEntry
 *
 * DESCRIPTION: Creates member list entry & inserts into vector
*/
void Node::insertEntry(std::vector<MemberListEntry>& memberList, Address& address, short port, long heartbeat, long timestamp, bool flag) {
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
		entry.timestamp = boost::chrono::duration_cast<boost::chrono::milliseconds>(boost::chrono::system_clock::now().time_since_epoch()).count();
		memberList.push_back(entry);
		//if(flag) {
		//	log->logNodeAdd(&memberNode->addr, &addedNode);
		//}
	}
	else {
		for(; it_beg != it_end; ++it_beg) 
		{
			if(address == (*it_beg).address)
			{
				if(entry.heartbeat > (*it_beg).heartbeat)
				{
					(*it_beg).timestamp = boost::chrono::duration_cast<boost::chrono::milliseconds>(boost::chrono::system_clock::now().time_since_epoch()).count();
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

bool Node::isNodeRemoved(Address& addr, int port) 
{
	std::vector<MemberListEntry>:: iterator it_beg = this->memberList.begin();
	std::vector<MemberListEntry>:: iterator it_end = this->memberList.end();
	for(; it_beg != it_end; ++it_beg) 
	{
		MemberListEntry entry = *it_beg;
		if(addr == entry.address && port == entry.port) 
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
    t1.join();
    t2.join();
	t3.join();
	
	return 0;
}
