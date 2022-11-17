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
//#include <xmlrpccpp.h>
using namespace std;

HooFSRPCClient::HooFSRPCClient(string ip, int serverPort) {
    serverURL = "http://" + ip + ":" + to_string(serverPort) + "/RPC2";
    
}

HooFSRPCClient::~HooFSRPCClient() {

}

dirent *HooFSRPCClient::readdir(const char *path) {

}

int HooFSRPCClient::open(const char *path, int flags) {

}

int HooFSRPCClient::create(const char *path, int mode) {
    int ret = -1;

    try {
        int fd;
        ourClient.call(serverURL, _create, "sii", &res, path, mode, fd);
        ret = value_int(res);
    }
    catch (exception const& e) {
        cerr << "Client threw error: " << e.what() << endl;
    } catch (...) {
        cerr << "Client threw unexpected error." << endl;
    }
    
    return ret;
}

int HooFSRPCClient::unlink(const char *path) {

}

int HooFSRPCClient::release(int fd) {

}

struct stat *HooFSRPCClient::getAttr(const char *path, struct stat *stbuf) {

}

int HooFSRPCClient::rmdir(const char *path) {

}

int HooFSRPCClient::read(int fd, int size, int offset) {

}

int HooFSRPCClient::write(int fd, const char *buf, int size, int offset) {

}

