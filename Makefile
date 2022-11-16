#INCLUDES = $(shell $(XMLRPC_C_CONFIG) abyss-server --cflags)
INCLUDES = `xmlrpc-c-config abyss-server --cflags` -Wall -pedantic -Wextra

#LIBS = $(shell $(XMLRPC_C_CONFIG) abyss-server --libs)
LIBS = `xmlrpc-c-config abyss-server --libs`

HooFSRPCServer.o:
		gcc -std=gnu11 -c $(INCLUDES) HooFSRPCServer.c

hoofsserver:
		gcc -std=gnu11 -o HooFSRPCServer $(LIBS) HooFSRPCServer.o

clean:
		rm HooFSRPCServer.o