#**********************
#*
#* Progam Name: CAN Protocol.
#*
#* Current file: Makefile
#* About this file: Build Script.
#* 

CFLAGS = -Wall -Wextra -std=c++11

DLLS = -lboost_system -lboost_thread -lboost_chrono -lpthread

all: can_server

can_server: server_session.o server.o memberlistentry.o member.o client.o
	g++ server_session.o server.o memberlistentry.o member.o client.o Node.cpp -o server ${DLLS} ${CFLAGS}

server_session.o: Server_Session.cpp Message.hpp
	g++ member.o Server_Session.cpp -c -o server_session.o ${DLLS} ${CFLAGS}

server.o: Server.hpp Server.cpp Message.hpp
	g++ Server.cpp -c -o server.o ${DLLS} ${CFLAGS}

client.o: Client.hpp
	g++ Client.cpp -c -o client.o ${BOOST_SYSTEM} ${DLLS} ${CFLAGS}

member.o: Member.cpp memberlistentry.o 
	g++ memberlistentry.o Member.cpp -c -o member.o ${CFLAGS}
	
memberlistentry.o: MemberListEntry.cpp MemberListEntry.hpp
	g++ MemberListEntry.cpp -c -o memberlistentry.o ${CFLAGS}
