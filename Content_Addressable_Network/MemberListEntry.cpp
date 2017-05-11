
/**********************************
 * FILE NAME: MemberListEntry.cpp
 * 
 * Created by Shriram Joshi on 5/1/17
 * DESCRIPTION: Definition of all MemberListEntry related class
 * 
 * Copyright © 2017 Balakrishna. All rights reserved.
 **********************************/

#include "MemberListEntry.hpp"
#include "Member.hpp"

Address::Address(char addrAIn, char addrBIn, char addrCIn, char addrDIn, short portIn) :
addrA(addrAIn), addrB(addrBIn), addrC(addrCIn), addrD(addrDIn), port(portIn) { }

Address::Address(const Address &anotherAddress)
{
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
	return (addrA == anotherAddress.addrA && addrB == anotherAddress.addrB &&
            addrC == anotherAddress.addrC && addrD == anotherAddress.addrD &&
            port == anotherAddress.port);
}

std::string Address::to_string()
{
	return std::string(std::string(std::to_string(int(addrA))) + "." + std::string(std::to_string(int(addrB))) + "." +
                       std::string(std::to_string(int(addrC))) + "." + std::string(std::to_string(int(addrD))));
}

std::string Address::port_to_string()
{
    std::stringstream ss;
    ss << port;
    return ss.str();
}

void Address::init(std::string addr, short port)
{
	char *token;
	char* ipArr = const_cast<char*>(addr.c_str());
    token = std::strtok(ipArr, ".");
    addrA = atoi(token);
    token = strtok(NULL, ".");
    addrB = atoi(token);
    token = strtok(NULL, ".");
    addrC = atoi(token);
    token = strtok(NULL, ".");
    addrD = atoi(token);
	this->port = port;    
}

/**
 * Constructor
 */
MemberListEntry::MemberListEntry(Address& addr, long heartbeat, long long timestamp): address(addr),
                                 heartbeat(heartbeat), timestamp(timestamp), bDeleted(false){ }

/**
 * Constuctor
 */
MemberListEntry::MemberListEntry(Address& addr): address(addr) { }

MemberListEntry::MemberListEntry(Address& addr, Zone& zone) : address(addr), zone(std::move(zone)) { }

MemberListEntry::MemberListEntry(Address& addr, long heartbeat, long long timestamp, Zone& zone)
	: address(std::move(addr)), heartbeat(std::move(heartbeat)), timestamp(std::move(timestamp)), zone(std::move(zone)) { }

/**
 * Copy constructor
 */
MemberListEntry::MemberListEntry(const MemberListEntry &anotherMLE)
{
    this->heartbeat = anotherMLE.heartbeat;
    this->address = anotherMLE.address;
    this->timestamp = anotherMLE.timestamp;
    this->zone = anotherMLE.zone;
}

/**
 * Assignment operator overloading
 */
MemberListEntry& MemberListEntry::operator =(const MemberListEntry &anotherMLE)
{
    MemberListEntry temp(anotherMLE);
    std::swap(heartbeat, temp.heartbeat);
    std::swap(address, temp.address);
    std::swap(timestamp, temp.timestamp);
    std::swap(zone, temp.zone);
    return *this;
}

Address& MemberListEntry::getAddress() { return address; }

long MemberListEntry::getheartbeat() { return heartbeat; }

long long MemberListEntry::gettimestamp() { return timestamp; }

Zone MemberListEntry::getZone() { return zone; }

void MemberListEntry::setAddress(Address& addr) { this->address = addr; }

void MemberListEntry::setheartbeat(long hearbeat) { this->heartbeat = hearbeat; }

void MemberListEntry::settimestamp(long long timestamp) { this->timestamp = timestamp; }

short MemberListEntry::findMinDistance(boost_geometry::point_xy<short> pt) { return zone.minDistance(pt); }

void MemberListEntry::setZone(boost_geometry::point_xy<short> c1, boost_geometry::point_xy<short> c2,
                              boost_geometry::point_xy<short> c3, boost_geometry::point_xy<short> c4)
{
    zone.setZone(c1, c2, c3, c4);
}
