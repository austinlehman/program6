#INCLUDES = $(shell $(XMLRPC_C_CONFIG) abyss-server --cflags)
INCLUDES = `xmlrpc-c-config abyss-server --cflags` -Wall -pedantic -Wextra

#LIBS = $(shell $(XMLRPC_C_CONFIG) abyss-server --libs)
LIBS = `xmlrpc-c-config abyss-server --libs`

LDFLAGS = "-L/home/csi/l/lehman/OS/programs/p6/lib   -lxmlrpc++ -lxmlrpc_util++ -lxmlrpc_server_abyss -lxmlrpc_server -lxmlrpc_abyss  -lxmlrpc_client -lxmlrpc  -lxmlrpc_xmlparse -lxmlrpc_xmltok -lxmlrpc_util -lpthread -I/home/csi/l/lehman/OS/programs/p6/include"

HooFSRPCServer.o:
		gcc -std=gnu11 -c $(INCLUDES) HooFSRPCServer.c

hoofsserver:
		gcc -std=gnu11 -o HooFSRPCServer $(LIBS) HooFSRPCServer.o

clean:
		rm HooFSRPCServer.o

		
