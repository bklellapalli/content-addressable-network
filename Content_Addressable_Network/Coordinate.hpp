//
//  Coordinate.hpp
//  Content_Addressable_Network
//
//  Created by Balakrishna Lellapalli on 4/29/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//
#ifndef Coordinate_hpp
#define Coordinate_hpp

class Coordinate
{
    private:
        double x,y;
    
    public:
        void setX(double xIn);
        void setY(double xIn);
        double getX(void);
        double getY(void);
};

#endif /* Coordinate_hpp */
