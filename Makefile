CFLAGS = ""

RPCDIR = $(CURDIR)/xmlrpc-c-1.54.06
INCLUDE = -I. -I$(CURDIR)/include

default: HooFSRPCTestDriver HooFSRPCServer

HooFSRPCServer: HooFSRPCServer.o
	gcc -o hoofsrpcserver HooFSRPCServer.o $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc_server_abyss.a $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc_server.a $(CURDIR)/xmlrpc-c-1.54.06/lib/abyss/src/libxmlrpc_abyss.a -lssl -lcrypto    $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc.a $(CURDIR)/xmlrpc-c-1.54.06/lib/expat/xmlparse/libxmlrpc_xmlparse.a $(CURDIR)/xmlrpc-c-1.54.06/lib/expat/xmltok/libxmlrpc_xmltok.a $(CURDIR)/xmlrpc-c-1.54.06/lib/libutil/libxmlrpc_util.a -lpthread

HooFSRPCTestDriver: HooFSRPCClient.o HooFSRPCTestDriver.o
	g++ -std=c++11 -o hoofsrpctestdriver HooFSRPCTestDriver.o HooFSRPCClient.o $(CURDIR)/xmlrpc-c-1.54.06/src/cpp/libxmlrpc_client++.a $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc_client.a $(CURDIR)/xmlrpc-c-1.54.06/src/cpp/libxmlrpc++.a $(CURDIR)/xmlrpc-c-1.54.06/lib/libutil++/libxmlrpc_util++.a $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc.a $(CURDIR)/xmlrpc-c-1.54.06/lib/expat/xmlparse/libxmlrpc_xmlparse.a $(CURDIR)/xmlrpc-c-1.54.06/lib/expat/xmltok/libxmlrpc_xmltok.a $(CURDIR)/xmlrpc-c-1.54.06/lib/libutil/libxmlrpc_util.a -lpthread  -lcurl   $(CURDIR)/xmlrpc-c-1.54.06/src/cpp/libxmlrpc_packetsocket.a 

HooFSRPCServer.o: HooFSRPCServer.c
	gcc -c -I. $(INCLUDE) HooFSRPCServer.c 

HooFSRPCTestDriver.o: HooFSRPCTestDriver.cpp 
	g++ -std=c++11 -c -I. $(INCLUDE) HooFSRPCTestDriver.cpp

HooFSRPCClient.o: HooFSRPCClient.cpp
	g++ -std=c++11 -c -I. $(INCLUDE) HooFSRPCClient.cpp 
clean:
	rm *.o hoofsrpcserver