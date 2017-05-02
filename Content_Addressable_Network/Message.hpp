//
//  Message.hpp
//  Content_Addressable_Network
//
//  Created by Balakrishna Lellapalli on 5/1/17.
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef Message_hpp
#define Message_hpp

enum MsgTypes
{
    JOINREQ,
    JOINREP,
    HEARTBEAT,
    DUMMYLASTMSGTYPE
};

/* STRUCT NAME: MessageHdr
 *
 * DESCRIPTION: Header and content of a message */
typedef struct MessageHdr
{
    enum MsgTypes msgType;
    
} MessageHdr;

#endif /* Message_hpp */
