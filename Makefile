# Author: Joshua Hunter and Austin Lehman
# Assignment: Program 6
# Class: CSI 4337

CFLAGS = -Wall -Wextra -Wconversion -Wno-unused-parameter -g `pkg-config fuse --cflags`
LDFLAGS = `pkg-config fuse --libs`

RPCDIR = $(CURDIR)/xmlrpc-c-1.54.06
INCLUDE = -I. -I$(CURDIR)/include

default: HooFSRPCTestClient HooFSRPCServer HooFSFuse

HooFSRPCServer: HooFSRPCServer.o
	gcc -o hoofsrpcserver HooFSRPCServer.o $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc_server_abyss.a $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc_server.a $(CURDIR)/xmlrpc-c-1.54.06/lib/abyss/src/libxmlrpc_abyss.a -lssl -lcrypto    $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc.a $(CURDIR)/xmlrpc-c-1.54.06/lib/expat/xmlparse/libxmlrpc_xmlparse.a $(CURDIR)/xmlrpc-c-1.54.06/lib/expat/xmltok/libxmlrpc_xmltok.a $(CURDIR)/xmlrpc-c-1.54.06/lib/libutil/libxmlrpc_util.a -lpthread

HooFSRPCTestClient: HooFSRPCClient.o HooFSRPCTestDriver.o
	g++ -std=c++11 -o hoofsrpctestclient HooFSRPCTestDriver.o HooFSRPCClient.o $(CURDIR)/xmlrpc-c-1.54.06/src/cpp/libxmlrpc_client++.a $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc_client.a $(CURDIR)/xmlrpc-c-1.54.06/src/cpp/libxmlrpc++.a $(CURDIR)/xmlrpc-c-1.54.06/lib/libutil++/libxmlrpc_util++.a $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc.a $(CURDIR)/xmlrpc-c-1.54.06/lib/expat/xmlparse/libxmlrpc_xmlparse.a $(CURDIR)/xmlrpc-c-1.54.06/lib/expat/xmltok/libxmlrpc_xmltok.a $(CURDIR)/xmlrpc-c-1.54.06/lib/libutil/libxmlrpc_util.a -lpthread  -lcurl   $(CURDIR)/xmlrpc-c-1.54.06/src/cpp/libxmlrpc_packetsocket.a 

HooFSFuse: HooFSFuse.o HooFSRPCClient.o
	g++ -std=c++11 -o hoofsfuse HooFSFuse.o HooFSRPCClient.o -lfuse $(CURDIR)/xmlrpc-c-1.54.06/src/cpp/libxmlrpc_client++.a $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc_client.a $(CURDIR)/xmlrpc-c-1.54.06/src/cpp/libxmlrpc++.a $(CURDIR)/xmlrpc-c-1.54.06/lib/libutil++/libxmlrpc_util++.a $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc.a $(CURDIR)/xmlrpc-c-1.54.06/lib/expat/xmlparse/libxmlrpc_xmlparse.a $(CURDIR)/xmlrpc-c-1.54.06/lib/expat/xmltok/libxmlrpc_xmltok.a $(CURDIR)/xmlrpc-c-1.54.06/lib/libutil/libxmlrpc_util.a -lpthread  -lcurl   $(CURDIR)/xmlrpc-c-1.54.06/src/cpp/libxmlrpc_packetsocket.a 

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