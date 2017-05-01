//
//  Server.hpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 4/29/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef Server_hpp
#define Server_hpp

/*
#include <string>
#include <map>
#include "Zone.hpp"
#include "Coordinate.hpp"
#include "Generator.hpp"
class Server
{
    private:
        std::string ipAddress;
        Zone zone;
        std::map<std::string, Zone> neighbors;
        bool isInNetwork;
        Cordinate randomCordinate;
    
    public:
        std::string getIPAddress();
        void setZone(Zone zone);
        Zone getZone();
        void addNeighbor(std::string ipAddress, Zone zone);
        std::map<std::string, Zone> getNeighbors();
        void removeNeighbor(std::string ipAddress);
        std::string join(std::string identifier);
        std::string displayInformation();
        bool leave();
        // bool insertFile(std::string keyword, std::string file, Server node);
        // bool searchFile(std::string keyword, std::string fileName, Server node);
};

*/

#include "Server_Session.hpp"
#include "Member.hpp"
#include <queue>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <boost/array.hpp>
#include <list>

using boost::asio::ip::tcp;

#define CAN_PORT 5555

class Server
{
private:
    tcp::endpoint endpoint;
    tcp::acceptor acc;
    tcp::socket socket;
    std::list<std::pair<std::string, int>> member_list;
    
public:
    explicit Server(boost::asio::io_service& io_service, int port, std::queue<q_elt>* mesQ) : endpoint{tcp::v4(), static_cast<unsigned short>(port)}, acc{io_service, endpoint}, socket{io_service}
    {
        run(mesQ);
    }
    Server(Server&) = delete;
    virtual ~Server(void);
    inline std::list<std::pair<std::string, int>> getMemberList(void) {return member_list;}
private:
    void run(std::queue<q_elt>* mesQ);
};

#endif /* Server_hpp */
