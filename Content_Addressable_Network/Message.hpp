#ifndef Message_hpp
#define Message_hpp

#include <cstddef>
#include <string>

enum MsgType
{
    HEARTBEAT,
    JOINREQ,
    JOINREP,
    LEAVEREQ,
    VIEWREQ,
    SENDFILE,
    SEARCHFILE
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
		std::string data;
		size_t size;
		
        q_elt(std::string buf, size_t len) : data(std::move(buf)), size(len) { }
		inline std::string getElement(void) { return data; }
		inline int getSize() { return (int)size; }
};

#endif /* Message_hpp */
