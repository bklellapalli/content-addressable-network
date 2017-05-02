//
//  Member.hpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef Member_hpp
#define Member_hpp

#include <queue>
#include <vector>
#include <string>
#include "MemberListEntry.hpp"

/**
 * CLASS NAME: q_elt (Q_ELEMENT)
 *
 * DESCRIPTION: Entry in the queue
 */
class q_elt {

public:
    void *elt;
    int size;
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
    //Address addr;
    
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
    
    // Membership table
    std::vector<MemberListEntry> memberList;
    
    // My position in the membership table
    std::vector<MemberListEntry>::iterator myPos;
    
    // Queue for failure detection messages
    std::queue<q_elt>* mesQ;
    
    // Default constructor
    Member();
    
    // copy constructor
    Member(const Member &anotherMember) = delete;
    
    // Assignment operator overloading
    Member& operator =(const Member &anotherMember) = delete;
    virtual ~Member() {delete mesQ;}
};

#endif /* Member_hpp */
