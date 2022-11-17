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
    
    const string _readdir = "rpc.readdir";
    const string _open = "rpc.open";
    const string _create = "rpc.create";
    const string _unlink = "rpc.unlink";
    const string _release = "rpc.release";
    const string _getAttr = "rpc.getattr";
    const string _rmdir = "rpc.rmdir";
    const string _read = "rpc.read";
    const string _write = "rpc.write";
    const string _setxattr = "rpc.setxattr";
    const string _chmod = "rpc.chmod";
    const string _chown = "rpc.chown";
    const string _utime = "rpc.utime";
    const string _truncate = "rpc.truncate";
    
    
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
