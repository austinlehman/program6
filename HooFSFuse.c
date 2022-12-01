/************************************************
 *
 * Author: Joshua Hunter and Austin Lehman
 * Assignment: Program 7
 * Class: CSI 4337
 *
 ************************************************/

/*
 Hoofs (pronounce Hooves) Filesystem
 gcc -Wall `pkg-config fuse --cflags --libs` hoofs.c -o hoofs
 */

#define FUSE_USE_VERSION 26
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

#include "HooFSRPCClient.h"

//Constants
const int NUM_ARGS = 4;
const int PROG_ARG = 0;
const int IP_ARG = 1;
const int PORT_ARG = 2;
const int PORTMIN = 0;
const int PORTMAX = 65535;


static char *fileSystemRoot;
HooFSRPCClient *rpcClient;

/*
 * Get file attributes
 */
static int hoofs_getattr(const char *path, struct stat *stbuf) {
    // Compute the full path name
    printf("getattr\n\n\n");
    
    //return rpcClient->getAttr(path, (struct stat *) stbuf);
    return 0;
}

static int hoofs_setxattr(const char *path, const char *name, const char *value,
                          size_t size, int flags) {
    printf("setxattr\n");
    return 0;
}

static int hoofs_chmod(const char *path, mode_t mode) {
    printf("chmod\n");
    return 0;
}

static int hoofs_chown(const char *path, uid_t uid, gid_t gid) {
    printf("chown\n");
    return 0;
}

static int hoofs_utime(const char *path, struct utimbuf *ubuf) {
    printf("utime\n");
    return 0;
}

static int hoofs_truncate(const char *path, off_t newsize) {
    printf("truncate\n");
    return 0;
}

static int hoofs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t
                         offset, struct fuse_file_info *fi) {
    printf("readdir\n");
    return 0;
}

static int hoofs_open(const char *path, struct fuse_file_info *fi) {
    // Compute the full path name
    printf("open\n");
    return 0;
}

static int hoofs_release(const char *path, struct fuse_file_info *fi) {
    printf("release\n");
    return 0;
}

static int hoofs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    printf("create\n");
    return 0;
}

static int hoofs_unlink(const char *path) {
    printf("unlink\n");
    return 0;
}

static int hoofs_rmdir(const char *path) {
    printf("rmdir\n");
    return hoofs_unlink(path);
}

static int hoofs_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi) {
    printf("read\n");
    return 0;
}

static int hoofs_write(const char *path, const char *buf, size_t size, off_t
                       offset, struct fuse_file_info *fi) {
    printf("write\n");
    return (int) 0;
}

static struct fuse_operations hoofs_oper;

static int myfs_opt_proc(void *data, const char *arg, int key, struct fuse_args
                         *outargs) {
    if (key == FUSE_OPT_KEY_NONOPT && fileSystemRoot == NULL) {
        fileSystemRoot = strdup(arg);
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < NUM_ARGS) {
        fprintf(stderr, "Usage: %s <server name/address> <server port> <fuse params>...\n", argv[0]);
        exit(1);
    }
    
    char *serverIP = argv[IP_ARG];
    
    hoofs_oper.getattr = hoofs_getattr;
    hoofs_oper.setxattr = hoofs_setxattr;
    hoofs_oper.chmod = hoofs_chmod;
    hoofs_oper.chown = hoofs_chown;
    hoofs_oper.utime = hoofs_utime;
    hoofs_oper.truncate = hoofs_truncate;
    hoofs_oper.readdir = hoofs_readdir;
    hoofs_oper.open = hoofs_open;
    hoofs_oper.release = hoofs_release;
    hoofs_oper.create = hoofs_create;
    hoofs_oper.unlink = hoofs_unlink;
    hoofs_oper.rmdir = hoofs_rmdir;
    hoofs_oper.read = hoofs_read;
    hoofs_oper.write = hoofs_write;
    
    
    //Register and validate port
    int serverPort;
    sscanf(argv[PORT_ARG], "%d", &serverPort);
    if(serverPort < PORTMIN || serverPort > PORTMAX) {
        fprintf(stderr, "ERROR: Server port %d invalid. Port must be in the range 0 - 65535\n", serverPort);
        exit(2);
    }
    
    //Initialize client for RPC communication
    rpcClient = new HooFSRPCClient(serverIP, serverPort);

    //rpcClient->readdir(argv[3]);
    //rpcClient->create("hi.txt", 0777);
    //char** fuseArgs = argv+2;
    
    char* fuseArgs[argc - 2];
    fuseArgs[0] = argv[0];
    
    for(int i = 1; i < argc - 2; i++) {
        fuseArgs[i] = argv[i + 2];
    }
    
    struct fuse_args args = FUSE_ARGS_INIT(argc - 2, fuseArgs);
    fuse_opt_parse(&args, NULL, NULL, myfs_opt_proc);
    return fuse_main(args.argc, args.argv, &hoofs_oper, NULL);
    //return 0;
}
