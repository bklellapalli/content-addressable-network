
/**********************************
 * FILE NAME: Member.cpp
 *
 * Created by Shriram Joshi on 5/1/17
 * DESCRIPTION: Definition of all Member related class
 *
 * Copyright © 2017 Balakrishna. All rights reserved.
 **********************************/

#include <ctime>
#include <boost/random.hpp>
#include "Member.hpp"

/**
 * Constructor
 */

short Member::generateRandomNumber()
{
    std::time_t now = std::time(0);
    boost::random::mt19937 gen{static_cast<std::uint32_t>(now)};
    boost::random::uniform_int_distribution<> dist{MIN_COORDINATE, MAX_COORDINATE};
    return dist(gen);
}

Member::Member(): inited(false), inGroup(false), bFailed(false), nnb(0), heartbeat(0), pingCounter(0), timeOutCounter(0)
{
    short x_axis = generateRandomNumber();
    short y_axis = generateRandomNumber();
    boost::geometry::assign_values(point, x_axis, y_axis);
}

/*
 * Destructor
 */
Member::~Member()
{
    
}

bool Member::isNeighbour(MemberListEntry entry)
{
    return self_zone.is_share_axis(entry.getZone());
}
