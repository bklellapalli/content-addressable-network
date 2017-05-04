//
//  Coordinate.cpp
//  Content_Addressable_Network
//
//  Created by Balakrishna Lellapalli on 5/4/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#include <boost/random.hpp>
#include <ctime>
#include "Coordinate.hpp"

Coordinate::Coordinate()
{
    std::time_t now_x = std::time(0);
    boost::random::mt19937 gen_x{static_cast<std::uint32_t>(now_x)};
    boost::random::uniform_int_distribution<> dist_x{1, 100};
    x = dist_x(gen_x);
    
    std::time_t now_y = std::time(0);
    boost::random::mt19937 gen_y{static_cast<std::uint32_t>(now_y)};
    boost::random::uniform_int_distribution<> dist_y{1, 100};
    y = dist_y(gen_y);
}

void Coordinate::setCoordinates(short xValue, short yValue)
{
    x = xValue;
    y = yValue;
}

short Coordinate::getX(void)
{
    return x;
}

short Coordinate::getY(void)
{
    return y;
}
