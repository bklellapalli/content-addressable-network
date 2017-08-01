#ifndef MEMBER_HPP
#define MEMBER_HPP

#include "MemberListEntry.hpp"
#include "Zone.hpp"
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <string>

#define MIN_COORDINATE 1
#define MAX_COORDINATE 100

namespace boost_geometry = boost::geometry::model::d2;

/**
 * CLASS NAME: Member
 *
 * DESCRIPTION: Class representing a member in the distributed system
 */
// Declaration and definition here
class Member 
{
    public:
        bool inited;
        bool inGroup;
        bool bFailed;
        int nnb;
        long heartbeat;
        int pingCounter;
        int timeOutCounter;
        Address self_address;
        std::string self_identifier;

        Zone self_zone;
        boost_geometry::point_xy<int> point;
		std::vector<MemberListEntry> memberList;
    
    public:
        Member();
		Member(const Member &anotherMember) = delete;
        Member& operator =(const Member &anotherMember) = delete;
		virtual ~Member();
    	std::string getLocalIpAddress(void);
        bool isNeighbour(Zone zone);
};

#endif /* MEMBER_HPP */
