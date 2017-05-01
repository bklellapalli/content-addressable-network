//
//  Operation.hpp
//  Content_Addressable_Network
//
//  Created by Balakrishna Lellapalli on 4/29/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef Operation_hpp
#define Operation_hpp

#include <string>
#include <map>
#include "Zone.hpp"
#inlcude "Server.hpp"
#include "Coordinate.hpp"
class Client
{
    public:
        void splitZone(Zone z1, Zone z2);
        Zone merzeZone(Zone z1, Zone z2);
        Server routeToClosestPeer(std::map<std::string, Zone> neighbors, Coordinate coordinate);
};

#endif /* Operation_hpp */
