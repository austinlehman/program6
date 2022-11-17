/************************************************
*
* Author: Joshua Hunter and Austin Lehman
* Assignment: Program 6
* Class: CSI 4337
*
************************************************/

#ifndef HooFSRPCClient_h
#define HooFSRPCClient_h

#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/stat.h>

#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>


using namespace std;

class HooFSRPCClient {
    int cPort;
    int serverPort;
    string serverIP;
    
    HooFSRPCClient(string ip, int serverPort, int clientPort);
    ~HooFSRPCClient();
    
    dirent *readdir(char *path);
    int open(char *path, int flags);
    int create(char *path, int mode);
    int unlink(char *path);
    int release(int fd);
    struct stat *getAttr(char *path, struct stat *stbuf);
    int rmdir(char *path);
    int read(int fd, int size, int offset);
    int write(int fd, char *buf, int size, int offset);
};

#endif /* HooFSRPCClient_h */
