//
//  MemberListEntry.hpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef MEMBERLISTENTRY_HPP
#define MEMBERLISTENTRY_HPP

#include "Zone.hpp"
#include <string>

/**
 * CLASS NAME: Address
 *
 * DESCRIPTION: Class representing the address of a single node
 */
class Address
{
    public:
        Address() { }
        Address(char addrAIn, char addrBIn, char addrCIn, char addrDIn, short portIn);
		Address(const Address &anotherAddress);
		Address& operator =(const Address &anotherAddress);
		bool operator ==(const Address &anotherAddress);
        std::string to_string();
        std::string port_to_string();
        void init();
    
    public:
        char addrA, addrB, addrC, addrD;
        short port;
};

/**
 * CLASS NAME: MemberListEntry
 *
 * DESCRIPTION: Entry in the membership list
 */
class MemberListEntry
{
    private:
        Address address;
    	
    public:
        MemberListEntry(Address&, short, long, long long);
        MemberListEntry(Address&, short port);
        MemberListEntry(Address&, short port, Zone zone);
        MemberListEntry(Address&, short, long, long long, Zone zone);
        MemberListEntry(): port(0), heartbeat(0), timestamp(0) {}
        MemberListEntry(const MemberListEntry &anotherMLE);
        MemberListEntry& operator =(const MemberListEntry&);
        Address& getAddress();
        short getport();
        long getheartbeat();
        long long gettimestamp();
        void setAddress(Address&);
        void setport(short port);
        void setheartbeat(long hearbeat);
        void settimestamp(long long timestamp);
    	void setCurrentZone(boost::geometry::model::d2::point_xy<int> c1, boost::geometry::model::d2::point_xy<int> c2, 
    	boost::geometry::model::d2::point_xy<int> c3, boost::geometry::model::d2::point_xy<int> c4);
		Zone getCurrentZone();
		short findMinDistance(boost::geometry::model::d2::point_xy<int> pt);
    public:
        short port;
        long heartbeat;
        long long timestamp;
        bool bDeleted;
        Zone current_zone;
};

#endif /* MEMBERLISTENTRY_HPP */
