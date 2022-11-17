/************************************************
*
* Author: Joshua Hunter and Austin Lehman
* Assignment: Program 6
* Class: CSI 4337
*
************************************************/

#ifndef HooFSRPCClient_h
#define HooFSRPCClient_h

//General Includes
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>
using namespace std;

//XMLRPC-C Includes
#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>
using namespace xmlrpc_c;

//Server action method links
string const _readdir("rpc.readdir");
string const _open("rpc.open");
string const _create("rpc.create");
string const _unlink("rpc.unlink");
string const _release("rpc.release");
string const _getAttr("rpc.getattr");
string const _rmdir("rpc.rmdir");
string const _read("rpc.read");
string const _write("rpc.write");
string const _setxattr("rpc.setxattr");
string const _chmod("rpc.chmod");
string const _chown("rpc.chown");
string const _utime("rpc.utime");
string const _truncate("rpc.truncate");

/**
 * HooFDRPCClient is the client to implement the 
 * \version 1.0
 */
class HooFSRPCClient {
    string serverURL;
    clientSimple ourClient;
    
    public:
        HooFSRPCClient(string ip, int serverPort);
        ~HooFSRPCClient();
        dirent *readdir(const char *path);
        int open(const char *path, int flags);
        int create(const char *path, int mode);
        int unlink(const char *path);
        int release(int fd);
        struct stat *getAttr(const char *path, struct stat *stbuf);
        int rmdir(const char *path);
        string read(int fd, int size, int offset);
        int write(int fd, int size, int offset, const char *data);
};

#endif /* HooFSRPCClient_h */
