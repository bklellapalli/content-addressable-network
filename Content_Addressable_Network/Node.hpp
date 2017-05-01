//
//  Node.hpp
//  Content_Addressable_Network
//
//  Created by Shriam Joshi on 5/1/17
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#include "Server.hpp"
#include "Member.hpp"
#include <boost/asio.hpp>
#include <iostream>
class Node : public Member
{
private:
    Server server;
    
public:
    Node(boost::asio::io_service& io_service, int port) : server {io_service, port, mesQ} { }
    void getMessages()
    {
        if(!mesQ->empty())
        {
            std::queue<q_elt> temp = *mesQ;
            while(!temp.empty())
            {
                char* data = (char*)temp.front().elt;
                std::cout << data << std::endl;
                temp.pop();
            }
        }
    }
    void init_mem_protocol(void);	//thread
};

#endif /* Node_hpp */
