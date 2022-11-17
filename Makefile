CFLAGS = ""

RPCDIR = $(CURDIR)/xmlrpc-c-1.54.06
INCLUDE = -I. -I$(CURDIR)/include

default: HooFSRPCTestDriver HooFSRPCServer

HooFSRPCServer: HooFSRPCServer.o
	gcc -o hoofsrpcserver HooFSRPCServer.o $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc_server_abyss.a $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc_server.a $(CURDIR)/xmlrpc-c-1.54.06/lib/abyss/src/libxmlrpc_abyss.a -lssl -lcrypto    $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc.a $(CURDIR)/xmlrpc-c-1.54.06/lib/expat/xmlparse/libxmlrpc_xmlparse.a $(CURDIR)/xmlrpc-c-1.54.06/lib/expat/xmltok/libxmlrpc_xmltok.a $(CURDIR)/xmlrpc-c-1.54.06/lib/libutil/libxmlrpc_util.a -lpthread

HooFSRPCTestDriver: HooFSRPCClient.o HooFSRPCTestDriver.o
	g++ -std=c++11 -o hoofsrpctestdriver HooFSRPCTestDriver.o HooFSRPCClient.o  -L/usr/local/lib -lxmlrpc_server_abyss++ -lxmlrpc_server++ -lxmlrpc_abyss++ -lxmlrpc_server_abyss -lxmlrpc_server -lxmlrpc_abyss -lssl -lcrypto -lxmlrpc_client++ -lxmlrpc_client -lxmlrpc++ -lxmlrpc_util++ -lxmlrpc  -lxmlrpc_xmlparse -lxmlrpc_xmltok -lxmlrpc_util -lpthread -lcurl -lxmlrpc_packetsocket $(INCLUDE)

HooFSRPCServer.o: HooFSRPCServer.c
	gcc -c -I. $(INCLUDE) HooFSRPCServer.c 

HooFSRPCTestDriver.o: HooFSRPCTestDriver.cpp 
	g++ -std=c++11 -c -I. $(INCLUDE) HooFSRPCTestDriver.cpp

HooFSRPCClient.o: HooFSRPCClient.cpp
	g++ -std=c++11 -c -I. $(INCLUDE) HooFSRPCClient.cpp 

clean:
	rm *.o hoofsrpcserver