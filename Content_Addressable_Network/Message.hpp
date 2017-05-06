//
//  Message.hpp
//  Content_Addressable_Network
//
//  Created by Balakrishna Lellapalli on 5/1/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef Message_hpp
#define Message_hpp

#include <cstddef>

enum MsgType
{
    JOINREQ,
    VIEWREQ,
    LEAVEREQ,
    SENDFILE,
    SEARCHFILE,
    HEARTBEAT
};

/* STRUCT NAME: MessageHdr
 * DESCRIPTION: Header and content of a message */
typedef struct MessageHdr
{
    enum MsgType msgType;
    
} MessageHdr;

/**
 * CLASS NAME: q_elt (Q_ELEMENT)
 *
 * DESCRIPTION: Entry in the queue
 */
class q_elt 
{
	public:
		char *elt;
		size_t size;
		q_elt(void* buf, size_t len) 
		{
			elt = (char*)buf;
			size = len;
		}
		inline void* getElement(void) {return elt;}
		inline int getSize() { return size; }
};

#endif /* Message_hpp */
