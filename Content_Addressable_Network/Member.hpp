//
//  Member.hpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 4/29/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef MEMBER_HPP
#define MEMBER_HPP

#include <queue>
#include "MemberListEntry.hpp"
#include "Coordinate.hpp"
#include "Zone.hpp"

/**
 * CLASS NAME: q_elt (Q_ELEMENT)
 *
 * DESCRIPTION: Entry in the queue
 */
class q_elt
{
    private:
        void *elt;
        int size;
    
    public:
        void* getElement();
        q_elt(void *elt, int size);
};

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
    
        // X and Y coordinate
        Coordinate coordinates;
    
        // Current Zone
        Zone zone;
	
    public:
        Member();
		// copy constructor
		Member(const Member &anotherMember) = delete;
		// Assignment operator overloading
		Member& operator =(const Member &anotherMember) = delete;
		virtual ~Member() {delete mesQ;}
    
        // Membership table
        std::vector<MemberListEntry> memberList;
    
        // My position in the membership table
        std::vector<MemberListEntry>::iterator myPos;
    
        // Queue for failure detection messages
        std::queue<q_elt>* mesQ;
};

#endif /* MEMBER_HPP */
