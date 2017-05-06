//
//  Node.hpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 4/29/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef NODE_HPP
#define NODE_HPP

#include "Server.hpp"
#include "Member.hpp"
#include "Message.hpp"
#include "Message.hpp"
#include <boost/asio.hpp>

#define TREMOVE 50
#define TFAIL 10

class Node : public Member
{
	private:
		Server* server;
		std::queue<q_elt>* sndMsgsQ;
		std::queue<q_elt>* rcvMsgsQ;
	public:
		Node(boost::asio::io_service& io_service, int port); 
		~Node();

		//thread function
		void init_mem_protocol(void);	
		void accept_user_input(void);
        void pushMessage(MsgType type);
        void recv(void);
		void sendLoop(void);
		
	private:
		void getMemberList(std::vector<MemberListEntry>&, char*, bool = false);
		bool isNodeRemoved(Address& addr, int port);
		void insertEntry(std::vector<MemberListEntry>& memberList, Address& address, short port, long heartbeat, long long timestamp);
		size_t size_of_message(MsgType type);
        void displayInfo(void);
        short getRandomReceivers(void);
        void fillMemberShipList(char* msg);
		
};

#endif  /* NODE_HPP */
