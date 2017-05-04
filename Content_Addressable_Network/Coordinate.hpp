//
//  Coordinate.hpp
//  Content_Addressable_Network
//
//  Created by Balakrishna Lellapalli on 4/29/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef COORDINATE_HPP
#define COORDINATE_HPP

class Coordinate
{
     private:
        short x, y;
    
    public:
        Coordinate();
        void setCoordinates(short xValue, short yValue);
        short getX(void);
        short getY(void);
};

#endif /* COORDINATE_HPP */
