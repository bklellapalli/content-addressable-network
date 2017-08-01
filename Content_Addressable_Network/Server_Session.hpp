#ifndef SERVER_SESSION_HPP
#define SERVER_SESSION_HPP

#include "Message.hpp"
#include "SharedQueue.hpp"
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::tcp;

class Server_Session : public std::enable_shared_from_this<Server_Session>
{
    private:
        enum { max_length = 1024 };
	  	boost::array<char, max_length> buf;
	  	tcp::socket socket_;

    public:
	  	Server_Session(tcp::socket socket);
	  	~Server_Session();
	  	void start(SharedQueue<q_elt>* mesQ);
    
    private:
        void do_read(SharedQueue<q_elt>* mesQ);
};

#endif /* SERVER_SESSION_HPP */
