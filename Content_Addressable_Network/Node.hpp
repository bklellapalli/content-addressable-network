#ifndef NODE_HPP
#define NODE_HPP

#include "Server.hpp"
#include "Member.hpp"
#include "Message.hpp"
#include "Logger.hpp"
#include "FileProcessor.hpp"
#include "SharedQueue.hpp"
#include <boost/asio.hpp>
#include <utility>

#define TREMOVE 50000
#define TFAIL 10000

namespace boost_geometry = boost::geometry::model::d2;

class Node : public Member
{
	private:
		Server* server;
		SharedQueue<std::pair<std::pair<std::string, std::string>, q_elt>>* sndMsgsQ;
		SharedQueue<q_elt>* rcvMsgsQ;
		
	public:
		Node(boost::asio::io_service& io_service, int port); 
		~Node();

		//thread functions
		void init_mem_protocol(void);	
		void accept_user_input(void);
        void pushMessage(MsgType type, Zone&,  std::string = "", std::string = "");
        void recv(void);
		void sendLoop(void);
		
	private:
		void getMemberList(char*);
		void insertEntry(std::vector<MemberListEntry>& memberList, Address& address, long heartbeat, long long timestamp);
		size_t size_of_message(MsgType type);
        void displayInfo(Address& addr, std::vector<MemberListEntry>& member_list);
        short getRandomReceivers(void);
        void fillMemberShipList(char* msg);
};

#endif  /* NODE_HPP */
