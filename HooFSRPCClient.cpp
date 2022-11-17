/************************************************
*
* Author: Joshua Hunter and Austin Lehman
* Assignment: Program 6
* Class: CSI 4337
*
************************************************/

#include "HooFSRPCClient.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <xmlrpccpp.h>
using namespace std;

HooFSRPCClient::HooFSRPCClient(string ip, int serverPort, int clientPort) {
    this->cPort = clientPort;
    this->serverPort = serverPort;
    this->serverIP = ip;
}

HooFSRPCClient::~HooFSRPCClient() {

}

dirent *readdir(char *path) {

}

int open(char *path, int flags) {

}

int create(char *path, int mode) {

}

int unlink(char *path) {

}

int release(int fd) {

}

struct stat *getAttr(char *path, struct stat *stbuf) {

}

int rmdir(char *path) {

}

int read(int fd, int size, int offset) {

}

int write(int fd, char *buf, int size, int offset) {

}