//
//  Message.hpp
//  Content_Addressable_Network
//
//  Created by Balakrishna Lellapalli on 5/1/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

enum MsgTypes
{
    JOINREQ, VIEWREQ, LEAVEREQ, SENDMSG, HEARTBEAT
};

/* STRUCT NAME: MessageHdr
 *
 * DESCRIPTION: Header and content of a message */
typedef struct MessageHdr
{
    enum MsgTypes msgType;
    
} MessageHdr;

#endif /* MESSAGE_HPP */
