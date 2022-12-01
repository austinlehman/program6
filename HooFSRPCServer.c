/************************************************
 *
 * Author: Joshua Hunter and Austin Lehman
 * Assignment: Program 6
 * Class: CSI 4337
 *
 ************************************************/

#define WIN32_LEAN_AND_MEAN  /* required by xmlrpc-c/server_abyss.h */

/* General includes */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utime.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/xattr.h>

#ifdef _WIN32
#  include <windows.h>
#  include <winsock2.h>
#else
#  include <unistd.h>
#endif

/* XMLRPC-C Includes */
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/server.h>
#include <xmlrpc-c/server_abyss.h>

#ifdef _WIN32
#define SLEEP(seconds) SleepEx(seconds * 1000, 1);
#else
#define SLEEP(seconds) sleep(seconds);
#endif

/* Constants */
const int NUM_ARGS = 1;
const int PORT_ARG = 1;
const int PORTMIN = 0;
const int PORTMAX = 65535;

/* Global variables */
static char *fileSystemRoot = "./";

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

    /* XMLRPC Attributes to unload parameter values into */
    xmlrpc_value* initPath;
    xmlrpc_value* initStBuf;

    /* Syphon off values from parameters */
    xmlrpc_decompose_value(envP, paramArrayP, "(sS)", &initPath, &initStBuf);
    if(envP->fault_occurred) {
        return NULL;
    }

    /* Unload from decomposition */
    const char *path = (char *)initPath;
    struct stat *stbuf = (struct stat *)initStBuf;
    size_t pathLen = getFullPathLength(path);
    char fullPath[pathLen];
    getFullPath(path, fullPath, pathLen);

    /* Get attributes */
    logMessage("Getting attributes for %s\n", fullPath);
    if (lstat(fullPath, stbuf) == -1) {
        logMessage("lstat() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    
    return xmlrpc_struct_new_value(envP, (xmlrpc_value *)stbuf);
}

static xmlrpc_value *rpc_setxattr(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {

    /* XMLRPC Attributes to unload parameter values into */
    xmlrpc_value *initPath;
    xmlrpc_value *initName;
    xmlrpc_value *initValue;
    xmlrpc_int initSize;
    xmlrpc_int initFlags;

    /* Syphon off values from parameters */
    xmlrpc_decompose_value(envP, paramArrayP, "(sssii)", &initPath, &initName, &initValue, &initSize, &initFlags);

    /* Unload from decomposition */
    const char *path = (char *)initPath;
    const char *name = (char *)initName;
    const char *value = (char *)initValue;
    size_t size = (size_t) initSize;
    int flags = (int) initFlags;
    
    logMessage("calling setxattr on %s with name %s, value %s, size %d, and flags %d", path, name, value, size, flags);
    
    /* Set attributes */
    if(setxattr(path, name, value, size,flags) < 0) {
        logMessage("setxattr() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    
    return xmlrpc_int_new(envP, 0);
}

static xmlrpc_value *rpc_chmod(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {

    /* XMLRPC Attributes to unload parameter values into */
    xmlrpc_value *initPath;
    xmlrpc_int initMode;

    /* Syphon off values from parameters */
    xmlrpc_decompose_value(envP, paramArrayP, "(si)", &initPath, &initMode);

    /* Unload from decomposition */
    const char *path = (char *) initPath;
    mode_t mode = (mode_t) initMode;

    /* Chmod */
    logMessage("calling chmod on %s with mode %d\n", path, mode);
    if(chmod(path, mode) < 0) {
        logMessage("chmod() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }

    return xmlrpc_int_new(envP, 0);
}

static xmlrpc_value *rpc_chown(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {

    /* XMLRPC Attributes to unload parameter values into */
    xmlrpc_value *initPath;
    xmlrpc_int initUID;
    xmlrpc_int initGID;

    /* Syphon off values from parameters */
    xmlrpc_decompose_value(envP, paramArrayP, "(sii)", &initPath, &initUID, &initGID);

    /* Unload from decomposition */
    const char *path = (char *)initPath;
    uid_t uid = (uid_t) initUID;
    gid_t gid = (gid_t) initGID;

    /* Chown */
    logMessage("Calling chown on %s\n", path);
    if(chown(path, uid, gid) < 0) {
        logMessage("chmod() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }

    return xmlrpc_int_new(envP, 0);
}

static xmlrpc_value *rpc_utime(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {

    /* XMLRPC Attributes to unload parameter values into */
    xmlrpc_value *initPath;
    xmlrpc_value *initUbuf;

    /* Syphon off values from parameters */
    xmlrpc_decompose_value(envP, paramArrayP, "(sS)", &initPath, &initUbuf);

    /* Unload from decomposition */
    const char *path = (char *)initPath;
    struct utimbuf *ubuf = (struct utimbuf *)initUbuf;

    /* Utime */
    logMessage("calling utime on %s", path);
    if(utime(path, ubuf) < 0) {
        logMessage("utime() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }

    return xmlrpc_int_new(envP, 0);
}

static xmlrpc_value *rpc_truncate(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {

    /* XMLRPC Attributes to unload parameter values into */
    xmlrpc_value *initPath;
    xmlrpc_int initNewSize;

    /* Syphon off values from parameters */
    xmlrpc_decompose_value(envP, paramArrayP, "(si)", &initPath, &initNewSize);

    /* Unload from decomposition */
    const char *path = (char *)initPath;
    off_t newsize = (off_t) initNewSize;
    size_t pathLen = getFullPathLength(path);
    char fullPath[pathLen];
    getFullPath(path, fullPath, pathLen);

    /* Truncate */
    logMessage("Truncating file/directory %s\n", fullPath);
    if(truncate(fullPath, newsize) < 0) {
        logMessage("truncate() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    
    return xmlrpc_int_new(envP, 0);
}

static xmlrpc_value *rpc_readdir(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {

    /* XMLRPC Attributes to unload parameter values into */
    xmlrpc_value *initPath;
    xmlrpc_value *initBuf;
    xmlrpc_int initOffset;
    xmlrpc_int *initFD;

    /* Syphon off values from parameters */
    xmlrpc_decompose_value(envP, paramArrayP, "(ssii)", &initPath, &initBuf, &initOffset, &initFD);

    /* Unload from decomposition */
    const char *path = (char *) initPath;
    void *buf = (char *) initBuf;
    off_t offset = (off_t) initOffset;
    int *fd = (int *)initFD;
    size_t pathLen = getFullPathLength(path);
    char fullPath[pathLen];
    getFullPath(path, fullPath, pathLen);

    /* Read from directory */
    logMessage("Reading directory for %s\n", fullPath);
    DIR *dirPtr = opendir(fullPath);
    if (dirPtr == NULL) {
        logMessage("opendir() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    struct dirent *dirEntry;
    while ((dirEntry = readdir(dirPtr)) != NULL) {
        strncat(buf, dirEntry->d_name, strlen(dirEntry->d_name)); //almost definitely wrong lol
    }
    closedir(dirPtr);

    return xmlrpc_int_new(envP, 0);
}

static xmlrpc_value *rpc_open(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {

    /* XMLRPC Attributes to unload parameter values into */
    xmlrpc_value *pathVal;
    xmlrpc_int fileFlags;

    /* Syphon off values from parameters */
    xmlrpc_decompose_value(envP, paramArrayP, "(si)", &pathVal, &fileFlags);
    const char *path = (char *)pathVal;
    unsigned int flags = (unsigned int) fileFlags;
    
    // Compute the full path name
    size_t pathLen = getFullPathLength(path);

    /* Unload from decomposition */
    char fullPath[pathLen];
    getFullPath(path, fullPath, pathLen);
    
    /* Open file based on specified path */
    logMessage("Opening file %s\n", fullPath);
    int fd = open(fullPath, flags);
    if (fd <= 0) {
        logMessage("open() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    
    return xmlrpc_int_new(envP, fd);
}

static xmlrpc_value *rpc_release(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {

    /* XMLRPC Attributes to unload parameter values into */
    xmlrpc_int initFd;

    /* Syphon off values from parameters */
    xmlrpc_decompose_value(envP, paramArrayP, "(i)", &initFd);

    /* Unload from decomposition */
    int fd = (int) initFd;

    /* Close file */
    logMessage("Closing file at %d\n", fd);
    if (close(fd) < 0) {
        logMessage("close() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    return xmlrpc_int_new(envP, 0);
}

static xmlrpc_value* rpc_create(xmlrpc_env *envP, xmlrpc_value *paramArrayP, void *serverInfo, void *callInfo) {

    /* XMLRPC Attributes to unload parameter values into */
    xmlrpc_value* initPath;
    xmlrpc_int initMode;

    /* Syphon off values from parameters */
    xmlrpc_decompose_value(envP, paramArrayP, "(si)", &initPath, &initMode);
    if(envP->fault_occurred) {
        return NULL;
    }

    /* Unload from decomposition */
    const char *path = (char *)initPath;
    mode_t mode = (mode_t) initMode;
    size_t pathLen = getFullPathLength(path);
    char fullPath[pathLen];
    getFullPath(path, fullPath, pathLen);

    /* Create file at specified path */
    logMessage("Creating file %s\n", fullPath);
    int fd = creat(fullPath, mode);
    if (fd < 0) {
        logMessage("creat() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    
    return xmlrpc_int_new(envP, 0);
}

static xmlrpc_value *rpc_unlink(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {

    /* XMLRPC Attributes to unload parameter values into */
    xmlrpc_value *initPath;

    /* Syphon off values from parameters */
    xmlrpc_decompose_value(envP, paramArrayP, "(s)", &initPath);

    /* Unload from decomposition */
    const char *path = (char *)initPath;
    size_t pathLen = getFullPathLength(path);
    char fullPath[pathLen];

    /* Identify full path */
    getFullPath(path, fullPath, pathLen);

    /* Unlink the file */
    logMessage("Deleting file/directory %s\n", fullPath);
    int retVal = remove(fullPath);
    if (retVal < 0) {
        logMessage("remove() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    
    return xmlrpc_int_new(envP, 0);
}

static xmlrpc_value* rpc_rename(xmlrpc_env *envP, xmlrpc_value *paramArrayP, void *serverInfo, void *callInfo) {

    /* XMLRPC Attributes to unload parameter values into */
    xmlrpc_value* initPath;
    xmlrpc_value* changePath;

    /* Syphon off values from parameters */
    xmlrpc_decompose_value(envP, paramArrayP, "(ss)", &initPath, &changePath);
    if(envP->fault_occurred) {
        return NULL;
    }

    /* Unload from decomposition */
    const char *path = (char *)initPath;
    const char *newPath = (char *)changePath;
    size_t pathLen = getFullPathLength(path);
    size_t newPathLen = getFullPathLength(newPath);
    char fullPath[pathLen];
    char newFullPath[newPathLen];
    getFullPath(path, fullPath, pathLen);
    getFullPath(newPath, newFullPath, newPathLen);

    /* Rename file at specified path to new path */
    logMessage("Renaming file @ %s to %s\n", fullPath, newFullPath);
    int ind = rename(fullPath, newFullPath);
    if (ind < 0) {
        logMessage("rename() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }

    return xmlrpc_int_new(envP, 0);
}

static xmlrpc_value *rpc_rmdir(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {
    return rpc_unlink(envP, paramArrayP, serverInfo, channelInfo);
}

static xmlrpc_value *rpc_read(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {

    /* XMLRPC Attributes to unload parameter values into */
    xmlrpc_int initFD;
    xmlrpc_int initSize;
    xmlrpc_int initOffset;

    /* Syphon off values from parameters */
    xmlrpc_decompose_value(envP, paramArrayP, "(iii)", &initFD, &initSize, &initOffset);

    /* Unload from decomposition */
    size_t size = (size_t) initSize;
    off_t offset = (off_t) initOffset;
    char buf[(int) size];
    
    int fd = (int) initFD;

    /* Seek to position in file */
    logMessage("Reading from open file\n");
    if (lseek(fd, offset, SEEK_SET) < 0) {
        logMessage("lseek() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }

    /* Read specified number of bytes */
    ssize_t readBytes = read(fd, buf, size);

    if (readBytes < 0) {
        logMessage("read() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }
    
    return xmlrpc_value_new(envP, xmlrpc_string_new(envP, buf));
}

static xmlrpc_value *rpc_write(xmlrpc_env *const envP,  xmlrpc_value *const paramArrayP, void *const serverInfo, void *const channelInfo) {

    /* XMLRPC Attributes to unload parameter values into */
    xmlrpc_int initFD;
    xmlrpc_int initSize;
    xmlrpc_int initOffset;
    xmlrpc_value *initData;

    /* Syphon off values from parameters */
    xmlrpc_decompose_value(envP, paramArrayP, "(isii)", &initFD, &initData, &initSize, &initOffset);

    /* Unload from decomposition */
    const char *data = (char *) initData;
    size_t size = (size_t) initSize;
    off_t offset = (off_t) initOffset;
    int fd = (int) initFD;

    /* Seek to position in file */
    logMessage("Writing %s to file descriptor %d\n", data, fd);
    if (lseek(fd, offset, SEEK_SET) < 0) {
        logMessage("lseek() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }

    /* Identify written bytes */
    ssize_t writtenBytes = write((int) fd, data, size);
    if (writtenBytes < 0) {
        logMessage("write() failed: %s\n", strerror(errno));
        return xmlrpc_int_new(envP, -errno);
    }

    return xmlrpc_int_new(envP, (int) writtenBytes);
}

int main(int argc, char *argv[]) {
    
    /* Check to make sure that a port is passed */
    if (argc - 1 != NUM_ARGS) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    
    /* RPC-XML Method Structures */
    struct xmlrpc_method_info3 const rpcgetattrMethodInfo = {
        .methodName = "rpc.getattr",
        .methodFunction = &rpc_getattr,
    };
    
    struct xmlrpc_method_info3 const rpcsetxattrMethodInfo = {
        .methodName = "rpc.setxattr",
        .methodFunction = &rpc_setxattr,
    };
    
    struct xmlrpc_method_info3 const rpcchmodMethodInfo = {
        .methodName = "rpc.chmod",
        .methodFunction = &rpc_chmod,
    };
    
    struct xmlrpc_method_info3 const rpcchownMethodInfo = {
        .methodName = "rpc.chown",
        .methodFunction = &rpc_chown,
    };
    
    struct xmlrpc_method_info3 const rpcutimeMethodInfo = {
        .methodName = "rpc.utime",
        .methodFunction = &rpc_utime,
    };
    
    struct xmlrpc_method_info3 const rpctruncateMethodInfo = {
        .methodName = "rpc.truncate",
        .methodFunction = &rpc_truncate,
    };

    struct xmlrpc_method_info3 const rpcrenameMethodInfo = {
            .methodName = "rpc.rename",
            .methodFunction = &rpc_rename,
    };
    
    struct xmlrpc_method_info3 const rpcreaddirMethodInfo = {
        .methodName = "rpc.readdir",
        .methodFunction = &rpc_readdir,
    };
    
    struct xmlrpc_method_info3 const rpcopenMethodInfo = {
        .methodName = "rpc.open",
        .methodFunction = &rpc_open,
    };
    
    struct xmlrpc_method_info3 const rpcreleaseMethodInfo = {
        .methodName = "rpc.release",
        .methodFunction = &rpc_release,
    };
    
    struct xmlrpc_method_info3 const rpccreateMethodInfo = {
        .methodName = "rpc.create",
        .methodFunction = &rpc_create,
    };
    
    struct xmlrpc_method_info3 const rpcunlinkMethodInfo = {
        .methodName = "rpc.unlink",
        .methodFunction = &rpc_unlink,
    };
    
    struct xmlrpc_method_info3 const rpcrmdirMethodInfo = {
        .methodName = "rpc.rmdir",
        .methodFunction = &rpc_rmdir,
    };
    
    struct xmlrpc_method_info3 const rpcreadMethodInfo = {
        .methodName = "rpc.read",
        .methodFunction = &rpc_read,
    };
    
    struct xmlrpc_method_info3 const rpcwriteMethodInfo = {
        .methodName = "rpc.write",
        .methodFunction = &rpc_write,
    };
    
    
    
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
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcgetattrMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcsetxattrMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcchmodMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcchownMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcutimeMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpctruncateMethodInfo);
    xmlrpc_registry_add_method3(&env, serverRegistry, &rpcrenameMethodInfo);
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
    serverParams.log_file_name = "./log";
    
    xmlrpc_server_abyss(&env, &serverParams, XMLRPC_APSIZE(log_file_name));
    
    return 0;
}
