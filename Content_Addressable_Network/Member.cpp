//
//  Member.cpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17
//  Copyright © 2017 Balakrishna. All rights reserved.
//

/**********************************
 * FILE NAME: Member.cpp
 *
 * DESCRIPTION: Definition of all Member related class
 **********************************/

#include "Member.hpp"
#include <utility>
#include <ctime>
#include <boost/random.hpp>
/**
 * Constructor
 */
Member::Member(): inited(false), inGroup(false), bFailed(false), nnb(0), heartbeat(0), pingCounter(0), timeOutCounter(0)
{
	std::time_t now_x = std::time(0);
    boost::random::mt19937 gen_x{static_cast<std::uint32_t>(now_x)};
    boost::random::uniform_int_distribution<> dist_x{1, 100};
    
    std::time_t now_y = std::time(0);
    boost::random::mt19937 gen_y{static_cast<std::uint32_t>(now_y)};
    boost::random::uniform_int_distribution<> dist_y{1, 100};
    
    boost::geometry::assign_values(point, dist_x(gen_x), dist_y(gen_y));
}

/*
 * Destructor
 */
Member::~Member()
{
}

bool Member::isNeighbour(MemberListEntry entry)
{
    return current_zone.is_share_axis(entry.getCurrentZone());
}
