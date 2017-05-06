//
//  Client.hpp
//  Content_Addressable_Network
//
//  Created by Balakrishna Lellapalli on 4/29/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef Client_hpp
#define Client_hpp

#include <string>
#include "Message.hpp"

class Client
{
    private:
        void openSocket(std::string ipAddress, long port, MessageHdr* msg);
        MessageHdr* constructMessage(std::string ipAddress, long port, MsgTypes msg);
    
    /*
        std::String getIPV4Address();
        void join(std::string ipAddress, std::string identifier);
        void viewInfo(std::string identifier);
        void leave();
        void insertFile(std::string keyword, std::string fileName);
        void searchFile(std::string keyword, std::string fileName);
     */
    
    public:
        void sendMessage();
};

#endif /* Client_hpp */
