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

#include "MemberListEntry.hpp"
#include "Member.hpp"
#include <utility>
#include <iostream>

/**
 * Constructor
 */
q_elt::q_elt(void *elt, int size): elt(elt), size(size) {}

/**
 * Constructor
 */
MemberListEntry::MemberListEntry(std::string& addr, short port, long heartbeat, long timestamp): address(std::move(addr)), port(port), heartbeat(heartbeat), timestamp(timestamp) {}

/**
 * Constuctor
 */
MemberListEntry::MemberListEntry(std::string& addr, short port): address(std::move(addr)), port(port) {}

/**
 * Copy constructor
 */
MemberListEntry::MemberListEntry(const MemberListEntry &anotherMLE) {
    this->heartbeat = anotherMLE.heartbeat;
    this->address = anotherMLE.address;
    this->port = anotherMLE.port;
    this->timestamp = anotherMLE.timestamp;
}

/**
 * Assignment operator overloading
 */
MemberListEntry& MemberListEntry::operator =(const MemberListEntry &anotherMLE) {
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
std::string& MemberListEntry::getAddress() {
    return address;
}

/**
 * FUNCTION NAME: getport
 *
 * DESCRIPTION: getter
 */
short MemberListEntry::getport() {
    return port;
}

/**
 * FUNCTION NAME: getheartbeat
 *
 * DESCRIPTION: getter
 */
long MemberListEntry::getheartbeat() {
    return heartbeat;
}

/**
 * FUNCTION NAME: gettimestamp
 *
 * DESCRIPTION: getter
 */
long MemberListEntry::gettimestamp() {
    return timestamp;
}

/**
 * FUNCTION NAME: setid
 *
 * DESCRIPTION: setter
 */
void MemberListEntry::setAddress(std::string& addr) {
    this->address = addr;
}

/**
 * FUNCTION NAME: setport
 *
 * DESCRIPTION: setter
 */
void MemberListEntry::setport(short port) {
    this->port = port;
}

/**
 * FUNCTION NAME: setheartbeat
 *
 * DESCRIPTION: setter
 */
void MemberListEntry::setheartbeat(long hearbeat) {
    this->heartbeat = hearbeat;
}

/**
 * FUNCTION NAME: settimestamp
 *
 * DESCRIPTION: setter
 */
void MemberListEntry::settimestamp(long timestamp) {
    this->timestamp = timestamp;
}
