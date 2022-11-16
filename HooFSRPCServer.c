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

//Constants
const int NUM_ARGS = 1;
const int PORT_ARG = 1;
const int PORTMIN = 0;
const int PORTMAX = 65535;

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
static size_t getFullPathLength(const char *path) {
  return strlen(fileSystemRoot) + strlen(path) + 1;
}

/*
 * Compute the full path from the root (fileSystemRoot + path).
 * We assume the user only wants up to n-1 bytes of the full path.
 */
static char *getFullPath(const char *path, char *fullPath, size_t n) {
  strncpy(fullPath, fileSystemRoot, n);
  strncat(fullPath, path, n);
  return fullPath;
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

static int rpc_setxattr(const char *path, const char *name, const char *value,
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

static xmlrpc_value *rpc_truncate(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {
    xmlrpc_value *initPath;
    xmlrpc_int *initNewSize;
    
    xmlrpc_decompose_value(envP, paramArrayP, "si", &initPath, &initNewSize);
    
    const char *path = (char *)initPath;
    off_t newsize = (off_t)(*initNewSize);
    
    size_t pathLen = getFullPathLength(path);
    char fullPath[pathLen];
    
    getFullPath(path, fullPath, pathLen);
    logMessage("Truncating file/directory %s\n", fullPath);
   
    if(truncate(fullPath, newsize) < 0) {
        logMessage("truncate() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    
    return xmlrpc_int_new(envP, 0);
}

static int rpc_readdir(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {
    xmlrpc_value *initPath;
    xmlrpc_value *initBuf;
    xmlrpc_int *initOffset;
    xmlrpc_int *initFD;
    
    xmlrpc_decompose_value(envP, paramArrayP, "ssii", &initPath, &initBuf, &initOffset, &initFD);
    
    const char *path = (char *) initPath;
    void *buf = (char *) initBuf;
    //fuse_fill_dir_t filler;
    off_t offset = (off_t) (*initOffset);
    //struct fuse_file_info *fi;
    int *fd = (int *)initFD;
    
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
        /*
        if (filler(buf, dirEntry->d_name, NULL, 0) != 0) {
            return -ENOMEM;
        }
         */
        strncat(buf, dirEntry->d_name, strlen(dirEntry->d_name)); //almost definitely wrong lol
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

    xmlrpc_parse_value(envP, paramArrayP, "(sii)", &initPath, &initMode, &fi);

    if(envP->fault_occurred) {
        return NULL;
    }

    const char *path = (char *)initPath;
    mode_t mode = (mode_t)initMode;

    size_t pathLen = getFullPathLength(path);
    char fullPath[pathLen];
    getFullPath(path, fullPath, pathLen);
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

static xmlrpc_value *rpc_read(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {
    xmlrpc_value *initPath;
    xmlrpc_value *initBuf;
    xmlrpc_int *initSize;
    xmlrpc_int *initOffset;
    xmlrpc_int *initFD;
    
    xmlrpc_decompose_value(envP, paramArrayP, "ssiii", &initPath, &initBuf, &initSize, &initOffset, &initFD);
    
    const char *path = (char *) initPath;
    char *buf = (char *) initBuf;
    size_t size = (size_t) (*initSize); //may need to be changed to unsigned int??
    off_t offset = (off_t) (*initOffset);
    int fd = (int) (*initFD); //may need to be an int *????
    
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

static xmlrpc_value *rpc_write(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {
    xmlrpc_value *initPath;
    xmlrpc_value *initBuf;
    xmlrpc_int *initSize;
    xmlrpc_int *initOffset;
    xmlrpc_int *initFD;
    
    xmlrpc_decompose_value(envP, paramArrayP, "ssiii", &initPath, &initBuf, &initSize, &initOffset, &initFD);
    
    const char *path = (char *) initPath;
    const char *buf = (char *) initBuf;
    size_t size = (size_t) (*initSize); //may need to be changed to unsigned int??
    off_t offset = (off_t) (*initOffset);
    int fd = (int) (*initFD); //may need to be an int *????
    
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

static struct xmlrpc_method_info3 const rpcgetattrMethodInfo = {
        .methodName = "rpc_getattr",
        .methodFunction = &rpc_getattr,
};

static struct xmlrpc_method_info3 const rpcsetxattrMethodInfo = {
        .methodName = "rpc_setxattr",
        .methodFunction = &rpc_setxattr,
};

static struct xmlrpc_method_info3 const rpcchmodMethodInfo = {
        .methodName = "rpc_chmod",
        .methodFunction = &rpc_chmod,
};

static struct xmlrpc_method_info3 const rpcchownMethodInfo = {
        .methodName = "rpc_chown",
        .methodFunction = &rpc_chown,
};

static struct xmlrpc_method_info3 const rpcutimeMethodInfo = {
        .methodName = "rpc_utime",
        .methodFunction = &rpc_utime,
};

static struct xmlrpc_method_info3 const rpctruncateMethodInfo = {
        .methodName = "rpc_truncate",
        .methodFunction = &rpc_truncate,
};

static struct xmlrpc_method_info3 const rpcreaddirMethodInfo = {
        .methodName = "rpc_readdir",
        .methodFunction = &rpc_readdir,
};

static struct xmlrpc_method_info3 const rpcopenMethodInfo = {
        .methodName = "rpc_open",
        .methodFunction = &rpc_open,
};

static struct xmlrpc_method_info3 const rpcreleaseMethodInfo = {
        .methodName = "rpc_release",
        .methodFunction = &rpc_release,
};

static struct xmlrpc_method_info3 const rpccreateMethodInfo = {
        .methodName = "rpc_create",
        .methodFunction = &rpc_create,
};

static struct xmlrpc_method_info3 const rpcunlinkMethodInfo = {
        .methodName = "rpc_unlink",
        .methodFunction = &rpc_unlink,
};

static struct xmlrpc_method_info3 const rpcrmdirMethodInfo = {
        .methodName = "rpc_rmdir",
        .methodFunction = &rpc_rmdir,
};

static struct xmlrpc_method_info3 const rpcreadMethodInfo = {
        .methodName = "rpc_read",
        .methodFunction = &rpc_read,
};

static struct xmlrpc_method_info3 const rpcwriteMethodInfo = {
        .methodName = "rpc_write",
        .methodFunction = &rpc_write,
};

int main(int argc, char *argv[]) {

    /* Check to make sure that a port is passed */
    if (argc - 1 == NUM_ARGS) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    /* Validate port */
    int serverPort;
    sscanf(argv[PORT_ARG], "%d", &serverPort);
    if(serverPort < PORTMIN || serverPort > PORTMAX) {
        fprintf(stderr, "ERROR: Server port %d invalid. Port must be in the range 0 - 65535\n", serverPort);
        exit(2);
    }

    /* XML-RPC Server Params */
    xmlrpc_server_abyss_parms serverParams;
    xmlrpc_registry* serverRegistry;
    xmlrpc_env env;

    /* Initialize environment */
    xmlrpc_env_init(&env);
    serverRegistry = xmlrpc_registry_new(&env);

    /* Append m3 RPC methods to registry */
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcgetattrMethodInfo);           //THIS MIGHT NEED TO CHANGE
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcsetxattrMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcchmodMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcchownMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcutimeMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpctruncateMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcreaddirMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcopenMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcreleaseMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpccreateMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcunlinkMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcrmdirMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcreadMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcwriteMethodInfo);

    /* Set server parameters */
    serverParams.registryP = serverRegistry;
    serverParams.port_number = serverPort;
    serverParams.log_file_name = "/tmp/xmlrpc_log";

    xmlrpc_server_abyss(&env, &serverParams, XMLRPC_APSIZE(log_file_name));

    return 0;
}
