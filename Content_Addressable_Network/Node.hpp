#ifndef NODE_HPP
#define NODE_HPP

#include <boost/asio.hpp>
#include "Server.hpp"
#include "Member.hpp"
#include "Message.hpp"

#define TREMOVE 50
#define TFAIL 10

namespace boost_geometry = boost::geometry::model::d2;

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
        void pushMessage(MsgType type, Zone zone);
        void recv(void);
		void sendLoop(void);
		
	private:
		void getMemberList(std::vector<MemberListEntry>&, char*, bool = false);
		bool isNodeRemoved(Address& addr);
		void insertEntry(std::vector<MemberListEntry>& memberList, Address& address,
                         long heartbeat, long long timestamp);
		size_t size_of_message(MsgType type);
        void displayInfo(void);
        int getRandomReceivers(void);
        void fillMemberShipList(char* msg);
};

#endif  /* NODE_HPP */
