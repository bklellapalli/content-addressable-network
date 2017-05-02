//
//  MemberListEntry.hpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef MemberListEntry_hpp
#define MemberListEntry_hpp

#include <iostream>

/**
 * CLASS NAME: MemberListEntry
 *
 * DESCRIPTION: Entry in the membership list
 */
class MemberListEntry {
    
public:
    std::string address;
    short port;
    long heartbeat;
    long timestamp;
    MemberListEntry(std::string&, short, long, long);
    MemberListEntry(std::string&, short port);
    MemberListEntry(): address(""), port(0), heartbeat(0), timestamp(0) {}
    MemberListEntry(const MemberListEntry &anotherMLE);
    MemberListEntry& operator =(const MemberListEntry&);
    std::string& getAddress();
    short getport();
    long getheartbeat();
    long gettimestamp();
    void setAddress(std::string&);
    void setport(short port);
    void setheartbeat(long hearbeat);
    void settimestamp(long timestamp);
};

#endif /* MemberListEntry_hpp */
