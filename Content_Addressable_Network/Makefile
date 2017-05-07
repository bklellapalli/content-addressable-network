#**********************
#*
#* Progam Name: CAN Protocol.
#*
#* Current file: Makefile
#* About this file: Build Script.
#* 

CFLAGS = -Wall -Wextra -std=c++11

DLLS = -lboost_system -lboost_thread -lboost_chrono -lpthread

LOG_DLLS = -DBOOST_LOG_DYN_LINK -lboost_log

all: can_server

can_server: zone.o memberlistentry.o member.o logger.o server_session.o server.o client.o
	g++ zone.o server_session.o server.o memberlistentry.o member.o client.o logger.o Node.cpp -o server ${DLLS} ${LOG_DLLS} ${CFLAGS}

server_session.o: Server_Session.cpp Message.hpp
	g++ member.o Server_Session.cpp -c -o server_session.o ${DLLS} ${CFLAGS}

server.o: Server.hpp Server.cpp Message.hpp
	g++ Server.cpp -c -o server.o ${DLLS} ${CFLAGS}

client.o: Client.hpp
	g++ Client.cpp -c -o client.o ${BOOST_SYSTEM} ${DLLS} ${CFLAGS}

member.o: Member.cpp memberlistentry.o zone.o 
	g++ memberlistentry.o zone.o Member.cpp -c -o member.o ${CFLAGS}
	
memberlistentry.o: MemberListEntry.cpp MemberListEntry.hpp
	g++ MemberListEntry.cpp -c -o memberlistentry.o ${CFLAGS}
	
zone.o: Zone.cpp Zone.hpp
	g++ Zone.cpp -c -o zone.o ${CFLAGS}
	
logger.o: Logger.cpp Logger.hpp
	g++ Logger.cpp -c -o logger.o ${DLLS} ${LOG_DLLS} ${CFLAGS}
	
clean:
	rm *.o
	rm server

