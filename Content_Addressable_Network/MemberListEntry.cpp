//
//  MemberListEntry.cpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17
//  Copyright © 2017 Balakrishna. All rights reserved.
//

/**********************************
 * FILE NAME: MemberListEntry.cpp
 *
 * DESCRIPTION: Definition of all Member related class
 **********************************/

#include <string>
#include "MemberListEntry.hpp"
#include "Member.hpp"


Address::Address(char addrAIn, char addrBIn, char addrCIn, char addrDIn, short portIn) :
addrA(addrAIn), addrB(addrBIn), addrC(addrCIn), addrD(addrDIn), port(portIn) { }

Address::Address(const Address &anotherAddress)
{
	// strcpy(addr, anotherAddress.addr);
	addrA = anotherAddress.addrA;
	addrB = anotherAddress.addrB;
	addrC = anotherAddress.addrC;
	addrD = anotherAddress.addrD;
	port = anotherAddress.port;
}

/**
 * Assignment operator overloading
 */
Address& Address::operator =(const Address& anotherAddress)
{
	// strcpy(addr, anotherAddress.addr);
	addrA = anotherAddress.addrA;
	addrB = anotherAddress.addrB;
	addrC = anotherAddress.addrC;
	addrD = anotherAddress.addrD;
	port = anotherAddress.port;
	return *this;
}

/**
 * Compare two Address objects
 * Return true/non-zero if they have the same ip address and port number 
 * Return false/zero if they are different 
 */
bool Address::operator ==(const Address& anotherAddress)
{
	return (addrA == anotherAddress.addrA && 
		addrB == anotherAddress.addrB && 
		addrC == anotherAddress.addrC && 
		addrD == anotherAddress.addrD && 
		port == anotherAddress.port);
}

std::string Address::to_string()
{
    return std::string(std::string(1, addrA) + "." +
                       std::string(1, addrB) + "." +
                       std::string(1, addrC) + "." +
                       std::string(1, addrD));
}
void Address::init()
{
    std::memset(&addrA, 0, sizeof(addrA));
    std::memset(&addrA, 0, sizeof(addrB));
    std::memset(&addrA, 0, sizeof(addrC));
    std::memset(&addrA, 0, sizeof(addrD));
}

/**
 * Constructor
 */
MemberListEntry::MemberListEntry(Address& addr, short port, long heartbeat, long long timestamp): address(addr), port(port), heartbeat(heartbeat), timestamp(timestamp) { }

/**
 * Constuctor
 */
MemberListEntry::MemberListEntry(Address& addr, short port): address(addr), port(port) { }

/**
 * Copy constructor
 */
MemberListEntry::MemberListEntry(const MemberListEntry &anotherMLE)
{
    this->heartbeat = anotherMLE.heartbeat;
    this->address = anotherMLE.address;
    this->port = anotherMLE.port;
    this->timestamp = anotherMLE.timestamp;
}

/**
 * Assignment operator overloading
 */
MemberListEntry& MemberListEntry::operator =(const MemberListEntry &anotherMLE)
{
    MemberListEntry temp(anotherMLE);
    std::swap(heartbeat, temp.heartbeat);
    std::swap(address, temp.address);
    std::swap(port, temp.port);
    std::swap(timestamp, temp.timestamp);
    return *this;
}

/**
 * FUNCTION NAME: getid
 *
 * DESCRIPTION: getter
 */
Address& MemberListEntry::getAddress()
{
    return address;
}

/**
 * FUNCTION NAME: getport
 *
 * DESCRIPTION: getter
 */
short MemberListEntry::getport()
{
    return port;
}

/**
 * FUNCTION NAME: getheartbeat
 *
 * DESCRIPTION: getter
 */
long MemberListEntry::getheartbeat()
{
    return heartbeat;
}

/**
 * FUNCTION NAME: gettimestamp
 *
 * DESCRIPTION: getter
 */
long MemberListEntry::gettimestamp()
{
    return timestamp;
}

/**
 * FUNCTION NAME: setid
 *
 * DESCRIPTION: setter
 */
void MemberListEntry::setAddress(Address& addr)
{
    this->address = addr;
}

/**
 * FUNCTION NAME: setport
 *
 * DESCRIPTION: setter
 */
void MemberListEntry::setport(short port)
{
    this->port = port;
}

/**
 * FUNCTION NAME: setheartbeat
 *
 * DESCRIPTION: setter
 */
void MemberListEntry::setheartbeat(long hearbeat)
{
    this->heartbeat = hearbeat;
}

/**
 * FUNCTION NAME: settimestamp
 *
 * DESCRIPTION: setter
 */
void MemberListEntry::settimestamp(long long timestamp)
{
    this->timestamp = timestamp;
}
