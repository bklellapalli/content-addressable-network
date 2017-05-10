
#ifndef MEMBERLISTENTRY_HPP
#define MEMBERLISTENTRY_HPP

#include <string>
#include "Zone.hpp"

namespace boost_geometry = boost::geometry::model::d2;

/**
 * CLASS NAME: Address
 *
 * DESCRIPTION: Class representing the address of a single node
 */
class Address
{
    public:
        unsigned char addrA, addrB, addrC, addrD;
        short port;
    
    public:
        Address() { }
        Address(char addrAIn, char addrBIn, char addrCIn, char addrDIn, short portIn);
		Address(const Address &anotherAddress);
		Address& operator =(const Address &anotherAddress);
		bool operator ==(const Address &anotherAddress);
        std::string to_string();
        std::string port_to_string();
        void init(std::string, short);
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
        long heartbeat;
        long long timestamp;
        bool bDeleted;
        Zone zone;
    
    public:
        MemberListEntry(Address&, long, long long);
        MemberListEntry(Address&);
        MemberListEntry(Address&, Zone zone);
        MemberListEntry(Address&, long, long long, Zone zone);
        MemberListEntry(): heartbeat(0), timestamp(0) {}
        MemberListEntry(const MemberListEntry &anotherMLE);
        MemberListEntry& operator =(const MemberListEntry&);
    
        Address& getAddress();
        long getheartbeat();
        long long gettimestamp();
        Zone getZone();
    
        void setAddress(Address&);
        void setheartbeat(long hearbeat);
        void settimestamp(long long timestamp);
    	void setZone(boost_geometry::point_xy<short> c1, boost_geometry::point_xy<short> c2,
                            boost_geometry::point_xy<short> c3, boost_geometry::point_xy<short> c4);
		
		short findMinDistance(boost_geometry::point_xy<short> pt);
};

#endif /* MEMBERLISTENTRY_HPP */
