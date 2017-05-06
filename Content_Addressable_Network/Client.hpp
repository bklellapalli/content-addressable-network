#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <boost/asio.hpp>
using boost::asio::ip::tcp;

class Client
{
	private:
		
		tcp::resolver::iterator endpoint_iterator;
		const boost::asio::io_service& io_service;
		tcp::socket socket;
		tcp::resolver resolver;
	
    public:
		Client(boost::asio::io_service& io_service, std::string& connectToAddr, std::string& connectToPort);
		void write(char* data);
	
    private:
		void resolve(std::string& connectToAddr, std::string& connectToPort);
};

#endif
