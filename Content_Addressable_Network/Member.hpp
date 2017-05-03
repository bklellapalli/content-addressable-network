#ifndef MEMBER_HPP
#define MEMBER_HPP

#include "MemberListEntry.hpp"
#include <queue>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>

/**
 * CLASS NAME: q_elt (Q_ELEMENT)
 *
 * DESCRIPTION: Entry in the queue
 */
class q_elt {
public:
	void *elt;
	int size;
	q_elt(void *elt, int size);
};

/**
 * CLASS NAME: Member
 *
 * DESCRIPTION: Class representing a member in the distributed system
 */
// Declaration and definition here
class Member 
{
	public:
		// This member's Address
		//Address addr;
		// boolean indicating if this member is up
		bool inited;
		// boolean indicating if this member is in the group
		bool inGroup;
		// boolean indicating if this member has failed
		bool bFailed;
		// number of my neighbors
		int nnb;
		// the node's own heartbeat
		long heartbeat;
		// counter for next ping
		int pingCounter;
		// counter for ping timeout
		int timeOutCounter;
		// Membership table
		std::vector<MemberListEntry> memberList;
		// My position in the membership table
		std::vector<MemberListEntry>::iterator myPos;
		// Queue for failure detection messages
		std::queue<q_elt>* mesQ;
		/**
		 * Constructor
		 */
		Member(): inited(false), inGroup(false), bFailed(false), nnb(0), heartbeat(0), pingCounter(0), timeOutCounter(0), mesQ(new std::queue<q_elt>()) {}
		// copy constructor
		Member(const Member &anotherMember) = delete;
		// Assignment operator overloading
		Member& operator =(const Member &anotherMember) = delete;
		virtual ~Member() {delete mesQ;}
};

#endif /* MEMBER_H_ */
