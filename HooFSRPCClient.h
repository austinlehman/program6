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
string const _rename("rpc.rename");

//stat names
string const _dev("st_dev");
string const _ino("st_ino");
string const _mode("st_mode");
string const _nlink("st_nlink");
string const _uid("st_uid");
string const _gid("st_gid");
string const _rdev("st_rdev");
string const _size("st_size");
string const _blksize("st_blksize");
string const _blocks("st_blocks");
string const _atime("st_atime");
string const _mtime("st_mtime");
string const _ctime("st_ctime");
string const _actime("actime");
string const _modtime("modtime");






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
        char *readdir(const char *path);

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
         * \return list of attributes
         */
        struct stat *getAttr(const char *path);

        /**
         * Sets attributes in the current directory path
         * \param path directory path to operate over
         * \param name figure name
         * \param name figure value
         * \param name figure size
         * \param name flags to be set
         * \return Success indicator
         */
        int setxattr(const char *path, const char *name, const char *value, size_t size, int flags);

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

        /**
         * Changes file permissions
         * \param path directory path to operate over
         * \param mode permission mode to change to
         * \return Success indicator
         */
        int chmod(const char *path, int mode);

        /**
         * Changes file owner or group
         * \param path directory path to operate over
         * \param uid new owner user id
         * \param gid new owner group id
         * \return Success indicator
         */
        int chown(const char *path, int uid, int gid);

        /**
         * Modify file access time
         * \param path directory path to operate over
         * \param uBuf structure with new file access info
         * \return Success indicator
         */
        int utime(const char *path, struct utimbuf *uBuf);

        /**
         * Shrink or extend file size
         * \param path directory path to operate over
         * \param newSize size of the new file
         * \return Success indicator
         */
        int trunc(const char *path, int newSize);

        /**
         * Rename a file from old path to new
         * \param path directory path to operate over
         * \param newPath new path of the renamed directory
         * \return Success indicator
         */
        int rename(const char *path, const char *newPath);
};

#endif /* HooFSRPCClient_h */
