//
//  Zone.cpp
//  Content_Addressable_Network
//
//  Created by Balakrishna Lellapalli on 5/4/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#include "Zone.hpp"

Zone::Zone()
{
    c1.setCoordinates(0, 0);
    c2.setCoordinates(100, 0);
    c3.setCoordinates(100, 100);
    c4.setCoordinates(0, 100);
}

bool Zone::isCoordinateInZone(Coordinate coordinate)
{
    return (c1.getX() <= coordinate.getX() && coordinate.getX() <= c2.getX() &&
            c1.getY() <= coordinate.getY() && coordinate.getY() <= c4.getY());
}
