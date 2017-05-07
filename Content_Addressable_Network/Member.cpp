
/**********************************
 * FILE NAME: Member.cpp
 *
 * Created by Shriram Joshi on 5/1/17
 * DESCRIPTION: Definition of all Member related class
 *
 * Copyright © 2017 Balakrishna. All rights reserved.
 **********************************/

#include "Member.hpp"
#include "Logger.hpp"
#include <ctime>
#include <boost/random.hpp>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

/**
 * Constructor
 */

short Member::generateRandomNumber()
{
    std::time_t now = std::time(0);
    boost::random::mt19937 gen{static_cast<std::uint32_t>(now)};
    boost::random::uniform_int_distribution<> dist{MIN_COORDINATE, MAX_COORDINATE};
    return dist(gen);
}

Member::Member(): inited(false), inGroup(false), bFailed(false), nnb(0), heartbeat(0), pingCounter(0), timeOutCounter(0)
{
    short x_axis = generateRandomNumber();
    short y_axis = generateRandomNumber();
    boost::geometry::assign_values(point, x_axis, y_axis);
    LOG_TRACE << point.x() << " , " << point.y();
}

/*
 * Destructor
 */
Member::~Member()
{
    
}

bool Member::isNeighbour(MemberListEntry entry)
{
    return self_zone.is_share_axis(entry.getZone());
}

std::string Member::getLocalIpAddress()
{
	struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            std::string addrBuf(addressBuffer);
            
            if(addrBuf.compare("127.0.0.1") != 0 && addrBuf.compare("1.1.1.1") != 0) {
            	//std::cout << "Debug " << addrBuf << std::endl;
            	if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
            	return addrBuf;
            }
        }
    }
    if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
    return std::string("");
}

