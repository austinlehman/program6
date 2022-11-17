CFLAGS = -Wall -pedantic -Wextra

RPCDIR = $(CURDIR)/xmlrpc-c-1.54.06
INCLUDE = -I. -I$(CURDIR)/include

HooFSRPCServer: HooFSRPCServer.o
	gcc -o hoofsrpcserver HooFSRPCServer.o $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc_server_abyss.a $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc_server.a $(CURDIR)/xmlrpc-c-1.54.06/lib/abyss/src/libxmlrpc_abyss.a -lssl -lcrypto    $(CURDIR)/xmlrpc-c-1.54.06/src/libxmlrpc.a $(CURDIR)/xmlrpc-c-1.54.06/lib/expat/xmlparse/libxmlrpc_xmlparse.a $(CURDIR)/xmlrpc-c-1.54.06/lib/expat/xmltok/libxmlrpc_xmltok.a $(CURDIR)/xmlrpc-c-1.54.06/lib/libutil/libxmlrpc_util.a -lpthread

HooFSRPCServer.o:
	gcc -c -I. $(INCLUDE) HooFSRPCServer.c 

HooFSRPCClient.o:
	g++ -c -I. $(INCLUDE) HooFSRPCClient.cpp 

clean:
	rm *.o hoofsrpcserver
