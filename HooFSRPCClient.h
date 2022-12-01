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
 * HooFDRPCClient is the client to implement the action
 * methods for XMLRPC
 * \version 1.0
 */
class HooFSRPCClient {
    private:
        string serverURL;           //URL for the server
        clientSimple ourClient;     //Client connection to operate RPC calls
    
    public:
        /**
         * Default Constructor for HooFSRPCClient
         * \param ip ipaddr for the server connection
         * \param serverPort port for the server connection
         */
        HooFSRPCClient(string ip, int serverPort);

        /**
         * Deconstructs HooFSRPCClient
         */
        ~HooFSRPCClient();

        /**
         * Reads a listing of the directory
         * \param path directory path to operate over
         * \return listing of the directory
         */
        string readdir(const char *path);

        /**
         * Opens file in the directory path
         * \param path directory path to operate over
         * \param flags file flags
         * \return file descriptor as file indicator
         */
        int open(const char *path, int flags);

        /**
         * Opens file in the directory path
         * \param path directory path to operate over
         * \param mode permissions of the file
         * \return file descriptor as file indicator
         */
        int create(const char *path, int mode);

        /**
         * Unlinks the directory path
         * \param path directory path to operate over
         * \return Success indicator
         */
        int unlink(const char *path);

        /**
         * Closes file in the directory path
         * \param fd file to close
         * \return file descriptor as file indicator
         */
        int release(int fd);

        /**
         * Gets attributes in the current directory path
         * \param path directory path to operate over
         * \param stbuf buffer
         * \return list of attributes
         */
        struct stat *getAttr(const char *path, struct stat *stbuf);

        /**
         * Removes directory at the directory path
         * \param path directory path to operate over
         * \return Success indicator
         */
        int rmdir(const char *path);

        /**
         * Reads specified number of bytes from a file
         * \param fd file to read from
         * \param size number of bytes to read
         * \param offset how far from the head of the file to start reading
         * \return String of read bytes
         */
        string read(int fd, int size, int offset);

        /**
         * Writes specified number of bytes to a file
         * \param fd file to write to
         * \param size number of bytes to write
         * \param offset how far from the head of the file to start writing
         * \param data specific data to write to file
         * \return Number of written bytes
         */
        int write(int fd, int size, int offset, const char *data);
};

#endif /* HooFSRPCClient_h */
