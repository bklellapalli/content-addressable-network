//
//  Node.hpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 4/29/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include <boost/asio.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include "Server.hpp"
#include "Member.hpp"
#include "Message.hpp"

class Node : public Member
{
	private:
		Server server;
		std::queue<q_elt>* sndMsgsQ;
		std::queue<q_elt>* rcvMsgsQ;
        size_t size_of_message(MsgTypes type);
        void displayInfo();
	
    public:
        Node(boost::asio::io_service& io_service, int port);
		~Node();
		//receive messages from clients
		void recvLoop();
		void sendLoop();
		//thread function
		void init_mem_protocol(void);
        void accept_user_input();
        void pushMessage(MsgTypes type);
		void getMemberList(std::vector<MemberListEntry>&, char*, bool = false);
		bool isNodeRemoved(Address& addr, int port);
		void insertEntry(std::vector<MemberListEntry>& memberList, Address& address, short port,
                         long heartbeat, long timestamp, bool flag = false);
    
};

#endif /* NODE_HPP */
