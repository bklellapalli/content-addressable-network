//
//  MemberListEntry.hpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef MemberListEntry_hpp
#define MemberListEntry_hpp

#include <string>
#include <cstring>
/**
 * CLASS NAME: Address
 *
 * DESCRIPTION: Class representing the address of a single node
 */
class Address {
	public:
		char addrA;
		char addrB;
		char addrC;
		char addrD;
		short port;
		Address() {}
		// Copy constructor
		Address(const Address &anotherAddress);
		 // Overloaded = operator
		Address& operator =(const Address &anotherAddress);
		bool operator ==(const Address &anotherAddress);
		std::string to_string() {
			return std::string(std::string(1, addrA) + "." + 
							   std::string(1, addrB) + "." +
							   std::string(1, addrC) + "." +
							   std::string(1, addrD));
		}
		void init() 
		{
			std::memset(&addrA, 0, sizeof(addrA));
			std::memset(&addrA, 0, sizeof(addrB));
			std::memset(&addrA, 0, sizeof(addrC));
			std::memset(&addrA, 0, sizeof(addrD));
		}
};

/**
 * CLASS NAME: MemberListEntry
 *
 * DESCRIPTION: Entry in the membership list
 */
class MemberListEntry {
    
public:
    Address address;
    short port;
    long heartbeat;
    long long timestamp;
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
};

#endif /* MemberListEntry_hpp */
