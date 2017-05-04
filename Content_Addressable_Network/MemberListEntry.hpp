//
//  MemberListEntry.hpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef MEMBERLISTENTRY_HPP
#define MEMBERLISTENTRY_HPP

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
        MemberListEntry(): port(0), heartbeat(0), timestamp(0) {}
        MemberListEntry(const MemberListEntry &anotherMLE);
        MemberListEntry& operator =(const MemberListEntry&);
        Address& getAddress();
        short getport();
        long getheartbeat();
        long gettimestamp();
        void setAddress(Address&);
        void setport(short port);
        void setheartbeat(long hearbeat);
        void settimestamp(long long timestamp);
    
    public:
        short port;
        long heartbeat;
        long long timestamp;
};

#endif /* MEMBERLISTENTRY_HPP */
