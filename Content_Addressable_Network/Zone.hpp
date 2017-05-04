//
//  Zone.hpp
//  Content_Addressable_Network
//
//  Created by Balakrishna Lellapalli on 4/29/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef ZONE_HPP
#define ZONE_HPP

#include <list>
#include "Coordinate.hpp"

class Zone
{
    private:
        Coordinate c1, c2, c3, c4;
    
    public:
        Zone();
        bool isCoordinateInZone(Coordinate coordinate);
    
        //bool hasZoneShareAxis(Zone zone, bool isExactlySame);
        //double findClosestCoordinate(Coordinate coordinate);
        //std::list<Coordinate> getAllCoordinates();
        //double calculateArea();
};

#endif /* ZONE_HPP */
