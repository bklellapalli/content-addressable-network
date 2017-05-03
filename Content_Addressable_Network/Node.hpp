#ifndef NODE_HPP
#define NODE_HPP

#include "Server.hpp"
#include "Member.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <vector>

enum MsgType
{
    JOINREQ,
    VIEWREQ,
    LEAVEREQ,
    SENDFILE,
    SEARCHFILE,
    HEARTBEAT
};

/* STRUCT NAME: MessageHdr
 *
 * DESCRIPTION: Header and content of a message */
typedef struct MessageHdr
{
    enum MsgType msgType;
    
} MessageHdr;

class Node : public Member
{
	private:
		Server server;
		std::queue<q_elt>* sndMsgsQ;
		std::queue<q_elt>* rcvMsgsQ;
	public:
		Node(boost::asio::io_service& io_service, int port) : server {io_service, port, rcvMsgsQ} 
		{
			sndMsgsQ = new std::queue<q_elt>();
			rcvMsgsQ = new std::queue<q_elt>(); 
		} 
		~Node();
		//receive messages from clients
		void recvLoop();
		void sendLoop();
		//thread function
		void init_mem_protocol(void);	
		void getMemberList(std::vector<MemberListEntry>&, char*, bool = false);
		bool isNodeRemoved(Address& addr, int port);
		void insertEntry(std::vector<MemberListEntry>& memberList, Address& address, short port, long heartbeat, long timestamp, bool flag = false);
};

#endif
