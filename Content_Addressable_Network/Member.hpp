#ifndef MEMBER_HPP
#define MEMBER_HPP

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <queue>
#include <string>
#include "MemberListEntry.hpp"
#include "Zone.hpp"

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
        std::string self_identifier;
        Address self_address;
        bool inited;
        bool inGroup;
        bool bFailed;
        int nnb;
        long heartbeat;
        int pingCounter;
        int timeOutCounter;
    
        boost_geometry::point_xy<int> point;
        Zone self_zone;
    
		std::vector<MemberListEntry> memberList;
    
    public:
        Member();
		Member(const Member &anotherMember) = delete;
        Member& operator =(const Member &anotherMember) = delete;
		virtual ~Member();
        bool isNeighbour(MemberListEntry entry);
    	std::string getLocalIpAddress(void);
    private:
        short generateRandomNumber();
};

#endif /* MEMBER_HPP */
