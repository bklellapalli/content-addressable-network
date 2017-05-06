//
//  Member.hpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 4/29/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef MEMBER_HPP
#define MEMBER_HPP

#include "MemberListEntry.hpp"
#include "Zone.hpp"
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <queue>

/**
 * CLASS NAME: Member
 *
 * DESCRIPTION: Class representing a member in the distributed system
 */
// Declaration and definition here
class Member 
{
    public:
        // This member's Address
        Address address;
    
        // send to address
        Address send_to_address;
    
        // boolean indicating if this member is up
        bool inited;
    
        // boolean indicating if this member is in the group
        bool inGroup;
    
        // boolean indicating if this member has failed
        bool bFailed;
    
        // number of my neighbors
        int nnb;
    
        // the node's own heartbeat
        long heartbeat;
    
        // counter for next ping
        int pingCounter;
    
        // counter for ping timeout
        int timeOutCounter;
    
        boost::geometry::model::d2::point_xy<int> point;
    
        // Current Zone
		Zone current_zone;
		
		// Membership table
        std::vector<MemberListEntry> memberList;
    
        // My position in the membership table
        std::vector<MemberListEntry>::iterator myPos;
	
    public:
        Member();
		// copy constructor
		Member(const Member &anotherMember) = delete;
		// Assignment operator overloading
		Member& operator =(const Member &anotherMember) = delete;
		virtual ~Member();
    	// Checks whether entry is my neighbour
        bool isNeighbour(MemberListEntry entry);
};

#endif /* MEMBER_HPP */
