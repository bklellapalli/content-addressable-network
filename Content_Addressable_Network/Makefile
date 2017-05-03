all: can_server

can_server:
	g++ -std=c++11 -Wall -Wextra Client.cpp Member.cpp MemberListEntry.cpp Server_Session.cpp Server.cpp Node.cpp -o server /usr/local/lib/libboost_system.a -lboost_system -lboost_thread -lboost_chrono -lpthread

server_session.o:Server_Session.cpp member.o
	g++ -std=c++11 -Wall -Wextra member.o Server_Session.cpp -o server_session.o /usr/local/lib/libboost_system.a -lboost_system -lboost_thread -lpthread

server.o:Server.cpp
	g++ -std=c++11 -Wall -Wextra Server.cpp -o server.o /usr/local/lib/libboost_system.a -lboost_system -lboost_thread -lpthread

client.o:Client.cpp
	g++ -std=c++11 -Wall -Wextra Client.cpp -o client.o /usr/local/lib/libboost_system.a -lboost_system -lboost_thread -lpthread

member.o:Member.cpp
	g++ -std=c++11 -Wall -Wextra memberlistentry.o Member.cpp -o member.o
	
memberlistentry.o:MemberListEntry.cpp
	g++ -std=c++11 -Wall -Wextra MemberListEntry.cpp -c -o memberlistentry.o
