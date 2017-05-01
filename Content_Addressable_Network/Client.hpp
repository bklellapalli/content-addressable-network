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
class Client
{
    private:
        std::String getIPV4Address();
        void join(std::string ipAddress, std::string identifier);
        void viewInfo(std::string identifier);
        void leave();
        void insertFile(std::string keyword, std::string fileName);
        void searchFile(std::string keyword, std::string fileName);
};

#endif /* Client_hpp */
