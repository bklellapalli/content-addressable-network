#ifndef Server_hpp
#define Server_hpp

#include "Server_Session.hpp"
#include "Message.hpp"
#include "SharedQueue.hpp"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Server
{
	private:
		tcp::endpoint endpoint;
		tcp::acceptor acc;
		tcp::socket socket;
		
	public:
		Server(boost::asio::io_service& io_service, int port, SharedQueue<q_elt>* mesQ);
		Server(Server&) = delete;
		virtual ~Server(void);
		
	private:
		void run(SharedQueue<q_elt>* mesQ);
};

#endif /* Server_hpp */
