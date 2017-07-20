# Content_Addressable_Network
Content Addressable Network (CAN)

The Content Addressable Network (CAN) is a distributed, decentralized P2P infrastructure that provides hash table functionality. 
CAN was one of the original four distributed hash table proposals, introduced concurrently with Chord, Pastry, and Tapestry.

Peer- to-peer file sharing systems such as Napster and Gnutella could potentially be improved by CAN. 
In these systems, files are stored at the end user machines (peers) rather than at a central server and, 
as opposed to the traditional client-server model, files are transferred directly between peers. 
Most of the peer-to-peer designs are not scalable.

Napster: Central server stores index of all files. User queries the server and obtains the IP address of machine 
which has the requested file. File is then downloaded directly from this machine. Napster uses peer- to-peer model for 
file transfer, locating a file is centralized. This makes it both expensive (to scale the central directory) and 
vulnerable (since there is a single point of failure).

Gnutella: Users in the network requests for a file are flooded with a certain scope. Flooding on every request is not 
scalable and, because the flooding has to be curtailed at some point, may fail to find the file in the system.

Peer-to-peer file transfer process is inherently scalable, but the hard part is finding the peer from whom to retrieve the file.
Thus, a scalable peer-to-peer system requires, at the very least, a scalable indexing mechanism. 
We call such indexing systems Content-Addressable Networks.

CANs resemble a hash table; the basic operations performed on a CAN are the insertion, lookup and deletion of node. 
CAN is composed of many individual nodes. Each CAN node stores a zone of entire hash table. 
Node holds information about “adjacent” zones. Requests (insert, lookup, or delete) are routed by intermediate CAN nodes. 
CAN design is completely distributed, scalable and fault-tolerant.
 
ROUTING
A CAN node maintains a coordinate routing table that holds the IP address and virtual coordinate zone of each of its 
immediate neighbors in the coordinate space. Two nodes are neighbors if their coordinate spans overlap. 
This neighbor state is sufficient to route between two arbitrary points in the space: A CAN message includes the destination
coordinates. Using its neighbor coordinate set, a node routes a message towards its destination by simple greedy forwarding 
to the neighbor with coordinates closest to the destination coordinates.

Different paths exist between two points in the space and so, even if one or more of a node’s neighbors were to crash,
a node can automatically route along the next best available path.
If a node loses all its neighbors in a certain direction then greedy forwarding may temporarily fail.


NODE JOINING
To join CAN, a joining node must:
1. Find a node already in the overlay network.
2. Identify a zone that can be split
3. Update the routing tables of nodes neighboring the newly split zone.


Finding a Zone
New node randomly chooses a point P and sends a JOIN request destined for point P . 
This message is sent into the CAN via any existing CAN node. Each CAN node then uses the CAN routing mechanism 
to forward the message, until it reaches the node in whose zone P lies. This current occupant node then splits its 
zone in half and assigns one half to the new node.

Joining the Routing
Once the node managing the destination point's zone receives the join request, it may honor the join request by 
splitting its zone in half, allocating itself the first half, and allocating the joining node the second half. 
If it does not honor the join request, the joining node keeps picking random points in the coordinate space and 
sending join requests directed to these random points until it successfully joins the network. 
After the zone split and allocation is complete, the neighboring nodes are updated with the coordinates of the 
two new zones and the corresponding IP addresses. Routing tables are updated and updates are propagated across the network.



NODE DEPARTING
To handle a node departing, CAN must
1. Identify a node is departing.
2. Have the departing node's zone merged or taken over by a neighboring node.
3. Update the routing tables across the network.

Detecting a node's departure can be done, for instance, via heartbeat messages that periodically broadcast routing table
information between neighbors. After a predetermined period of silence from a neighbor, that neighboring node is determined
as failed and is considered a departing node. Alternatively, a node that is willingly departing may broadcast leave message
to its neighbors.

After a departing node is identified, its zone must be merged. First the departed node's zone is analyzed to determine
whether a neighboring node's zone can merge with the departed node's zone to form a valid zone. If one of the potential 
merges is deemed a valid merge, the zones are then merged. If none of the potential merges are deemed valid, then the
neighboring node with the smallest zone takes over control of the departing node's zone.

If the merge is successful, routing tables of neighboring zones' nodes are updated to reflect the merge. 
The network will see the subsection of the overlay network as one, single zone after a merge and treat all routing
processing with this mindset. To effectuate a take-over, the take-over node updates neighboring zones' nodes' routing tables,
so that requests to either zone resolve to the take-over node.



