/************************************************
 *
 * Author: Joshua Hunter and Austin Lehman
 * Assignment: Program 6
 * Class: CSI 4337
 *
 ************************************************/

#define WIN32_LEAN_AND_MEAN  /* required by xmlrpc-c/server_abyss.h */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#ifdef _WIN32
#  include <windows.h>
#  include <winsock2.h>
#else
#  include <unistd.h>
#endif

#include <xmlrpc.h>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/server.h>
#include <xmlrpc-c/server_abyss.h>

//#include "config.h"  /* information about this build environment */
#ifdef _WIN32
#define SLEEP(seconds) SleepEx(seconds * 1000, 1);
#else
#define SLEEP(seconds) sleep(seconds);
#endif

static char *fileSystemRoot;

/*
 * Write given log message to console
 */
static void logMessage(const char *format, ...) {
    va_list argList;
    va_start(argList, format);
    vfprintf(stdout, format, argList);
}

/*
 * Compute the byte length of the full path string
 * from the root (fileSystemRoot + path)
 */
static xmlrpc_value *getFullPathLength(xmlrpc_env *envP, const char *path) {                     //THESE WILL NEED TO BE UPDATED!!!!!!
    size_t length = strlen(fileSystemRoot) + strlen(path) + 1;

    xmlrpc_value* xmlLength = xmlrpc_i8_new(envP, length);
    return xmlLength;
}

/*
 * Compute the full path from the root (fileSystemRoot + path).
 * We assume the user only wants up to n-1 bytes of the full path.
 */
static xmlrpc_value *getFullPath(xmlrpc_env *envP, const char *path, char *fullPath, size_t n) {       //THESE WILL NEED TO BE UPDATED!!!!!!
    strncpy(fullPath, fileSystemRoot, n);
    strncat(fullPath, path, n);

    xmlrpc_value* xmlPath = xmlrpc_string_new(envP, fullPath);

    return xmlPath;
}

/*
 * Get file attributes
 */
static xmlrpc_value* rpc_getattr(xmlrpc_env* envP, xmlrpc_value* paramArrayP, void* serverInfo, void* callInfo) {
    
    xmlrpc_value* initPath;
    xmlrpc_value* initStBuf;
    
    xmlrpc_parse_value(envP, paramArrayP, "(sS)", &initPath, &initStBuf);
    if(envP->fault_occurred) {
        return NULL;
    }
    
    const char *path = (char *)initPath;
    struct stat *stbuf = (struct stat *)initStBuf;
    
    size_t pathLen = getFullPathLength(path);
    char fullPath[pathLen];
    
    getFullPath(path, fullPath, pathLen);
    
    logMessage("Getting attributes for %s\n", fullPath);
    if (lstat(fullPath, stbuf) == -1) {
        logMessage("lstat() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    
    return xmlrpc_int_new(envP, 0);
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
    // Compute the full path name
    size_t pathLen = getFullPathLength(path);
    char fullPath[pathLen];
    getFullPath(path, fullPath, pathLen);
    logMessage("Reading directory for %s\n", fullPath);
    // Fetch the directory information
    DIR *dirPtr = opendir(fullPath);
    if (dirPtr == NULL) {
        logMessage("opendir() failed: %s\n", strerror(errno));
        return -errno;
    }
    struct dirent *dirEntry;
    while ((dirEntry = readdir(dirPtr)) != NULL) {
        if (filler(buf, dirEntry->d_name, NULL, 0) != 0) {
            return -ENOMEM;
        }
    }
    closedir(dirPtr);
    return 0;
}

static xmlrpc_value *rpc_open(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {
    xmlrpc_value *pathVal;
    xmlrpc_int *fileInfo;
    xmlrpc_int *fileFlags;
    
    
    xmlrpc_decompose_value(envP, paramArrayP, "sii", &pathVal, &fileInfo, &fileFlags);
    const char *path = (char *)pathVal;
    int *fi = (int *) fileInfo;
    unsigned int flags = *((int *) fileFlags);
    
    // Compute the full path name
    size_t pathLen = getFullPathLength(path);

    char fullPath[pathLen];
    getFullPath(path, fullPath, pathLen);


    logMessage("Opening file %s\n", fullPath);
    int fd = open(fullPath, flags);
    if (fd <= 0) {
        logMessage("open() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    *fi = fd;
    return xmlrpc_int_new(envP, 0);
}

static xmlrpc_value *rpc_release(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {
    
    xmlrpc_value *initPath;
    xmlrpc_int *initFd;
    
    xmlrpc_decompose_value(envP, paramArrayP, "si", &initPath, &initFd);
    
    const char *path = (char *) initPath;
    int fd = *((int *) initFd);
    
    if (close(fd) < 0) {
        logMessage("closeOB() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    return xmlrpc_int_new(envP, 0);
}

static xmlrpc_value* rpc_create(xmlrpc_env *envP, xmlrpc_value *paramArrayP, void *serverInfo, void *callInfo) {

    xmlrpc_value* initPath;
    xmlrpc_value* initMode;
    xmlrpc_int* fi;

    xmlrpc_parse_value(envP, paramArrayP, "(sii)", initPath, initMode, fi);

    if(envP->fault_occurred) {
        return NULL;
    }

    const char *path = (char *)initPath;
    mode_t mode = (mode_t)initMode;

    size_t pathLen = getFullPathLength(envP, path);
    char fullPath[pathLen];
    getFullPath(envP, path, fullPath, pathLen);
    logMessage("Creating file %s\n", fullPath);

    int fd = creat(fullPath, mode);

    if (fd < 0) {
        logMessage("creat() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    fi = (xmlrpc_uint64_t*) &fd;

    return xmlrpc_int_new(envP, 0);
}

static xmlrpc_value *rpc_unlink(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {
    
    xmlrpc_value *initPath;
    xmlrpc_decompose_value(envP, paramArrayP, "s", &initPath);
    
    const char *path = (char *)initPath;
    
    
    size_t pathLen = getFullPathLength(path);
    char fullPath[pathLen];
    
    getFullPath(path, fullPath, pathLen);
    logMessage("Deleting file/directory %s\n", fullPath);
    
    int retVal = remove(fullPath);
    if (retVal < 0) {
        logMessage("remove() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    
    return xmlrpc_int_new(envP, 0);
}

static xmlrpc_value *rpc_rmdir(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {
    return rpc_unlink(envP, paramArrayP, serverInfo, channelInfo);
}

static xmlrpc_value *rpc_read(xmlrpc_env *envP, const char *path, char *buf, size_t size, off_t offset, int fd) {
    logMessage("Reading from open file\n");
    // Go to file offset
    if (lseek((int) fd, offset, SEEK_SET) < 0) {
        logMessage("lseek() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    // Read bytes
    ssize_t readBytes = read((int) fd, buf, size);
    if (readBytes < 0) {
        logMessage("read() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    return xmlrpc_int_new(envP, (int) readBytes);
}

static xmlrpc_value *rpc_write(xmlrpc_env *envP, const char *path, const char *buf, size_t size, off_t
                               offset, int fd) {
    logMessage("Writing to file\n");
    // Go to file offset
    if (lseek((int) fd, offset, SEEK_SET) < 0) {
        logMessage("lseek() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    // Write bytes
    ssize_t writtenBytes = write((int) fd, buf, size);
    if (writtenBytes < 0) {
        logMessage("write() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    return xmlrpc_int_new(envP, (int) writtenBytes);
}

static int myfs_opt_proc(void *data, const char *arg, int key, struct fuse_args
                         *outargs) {
    if (key == FUSE_OPT_KEY_NONOPT && fileSystemRoot == NULL) {
        fileSystemRoot = strdup(arg);
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <filesystem root> <fuse params>...\n", argv[0]);
        exit(1);
    }
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    fuse_opt_parse(&args, NULL, NULL, myfs_opt_proc);
    return fuse_main(args.argc, args.argv, &hoofs_oper, NULL);
}
