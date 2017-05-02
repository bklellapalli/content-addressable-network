//
//  Queue.hpp
//  Content_Addressable_Network
//
//  Created by Shriram Joshi on 5/1/17
//  Copyright © 2017 Balakrishna. All rights reserved.
//

#ifndef Queue_hpp
#define Queue_hpp

#include "Member.hpp"
#include <queue>

/**
 * Class name: Queue
 *
 * Description: This function wraps std::queue related functions
 */
class Queue {

public:
    Queue() { }
    virtual ~Queue() { }
    static bool enqueue(std::queue<q_elt> *queue, void *buffer, int size)
    {
        q_elt element(buffer, size);
        queue->emplace(element);
        return true;
    }
};

#endif /* Queue_hpp */
