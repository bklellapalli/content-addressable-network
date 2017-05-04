//
//  Member.cpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17
//  Copyright © 2017 Balakrishna. All rights reserved.
//

/**********************************
 * FILE NAME: Member.cpp
 *
 * DESCRIPTION: Definition of all Member related class
 **********************************/

#include "Member.hpp"

/**
 * Constructor
 */
q_elt::q_elt(void *elt, int size): elt(elt), size(size) { }

void* q_elt::getElement()
{
    return elt;
}

/**
 * Constructor
 */
Member::Member(): inited(false), inGroup(false), bFailed(false), nnb(0), heartbeat(0),
pingCounter(0), timeOutCounter(0), mesQ(new std::queue<q_elt>()) { }
