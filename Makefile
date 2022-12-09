# Author: Joshua Hunter and Austin Lehman
# Assignment: Program 6
# Class: CSI 4337

CFLAGS = -Wall -Wextra -Wconversion -Wno-unused-parameter -g `pkg-config fuse --cflags`
LDFLAGS = `pkg-config fuse --libs`

LIB = $(CURDIR)/lib
INCLUDE = -I. -I$(CURDIR)/include

default: HooFSRPCTestClient HooFSRPCServer HooFSFuse

HooFSRPCServer: HooFSRPCServer.o
	gcc -o hoofsrpcserver HooFSRPCServer.o $(LIB)/libxmlrpc_server_abyss.a $(LIB)/libxmlrpc_server.a $(LIB)/libxmlrpc_abyss.a -lssl -lcrypto	$(LIB)/libxmlrpc.a $(LIB)/libxmlrpc_xmlparse.a $(LIB)/libxmlrpc_xmltok.a $(LIB)/libxmlrpc_util.a -lpthread

HooFSRPCTestClient: HooFSRPCClient.o HooFSRPCTestDriver.o
	g++ -std=c++11 -o hoofsrpctestclient HooFSRPCTestDriver.o HooFSRPCClient.o $(LIB)/libxmlrpc_client++.a $(LIB)/libxmlrpc_client.a $(LIB)/libxmlrpc++.a $(LIB)/libxmlrpc_util++.a $(LIB)/libxmlrpc.a $(LIB)/libxmlrpc_xmlparse.a $(LIB)/libxmlrpc_xmltok.a $(LIB)/libxmlrpc_util.a -lpthread  -lcurl   $(LIB)/libxmlrpc_packetsocket.a 

HooFSFuse: HooFSFuse.o HooFSRPCClient.o
	g++ -std=c++11 -o hoofsfuse HooFSFuse.o HooFSRPCClient.o -lfuse $(LIB)/libxmlrpc_client++.a $(LIB)/libxmlrpc_client.a $(LIB)/libxmlrpc++.a $(LIB)/libxmlrpc_util++.a $(LIB)/libxmlrpc.a $(LIB)/libxmlrpc_xmlparse.a $(LIB)/libxmlrpc_xmltok.a $(LIB)/libxmlrpc_util.a -lpthread  -lcurl   $(LIB)/libxmlrpc_packetsocket.a 

HooFSRPCServer.o: HooFSRPCServer.c
	gcc -c -I. $(INCLUDE) HooFSRPCServer.c

HooFSRPCTestDriver.o: HooFSRPCTestDriver.cpp 
	g++ $(CFLAGS) -std=c++11 -c -I. $(INCLUDE) HooFSRPCTestDriver.cpp

HooFSRPCClient.o: HooFSRPCClient.cpp
	g++ $(CFLAGS) -std=c++11 -c -I. $(INCLUDE) HooFSRPCClient.cpp 

HooFSFuse.o: HooFSFuse.c
	g++ $(CFLAGS) -c -I. $(INCLUDE) HooFSFuse.c 

clean:
	rm *.o hoofsrpcserver hoofsrpctestclient hoofsfuse