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

dirent *HooFSRPCClient::readdir(const char *path) {                 //FIX
    xmlrpc_c::value response;
    ourClient.call(serverURL, _readdir, "", &response);
    cout << xmlrpc_c::value_int(response) << endl;
}

int HooFSRPCClient::open(const char *path, int flags) {
    xmlrpc_c::value response;
    ourClient.call(serverURL, _open, "si", &response, path, flags);

    int retFile = xmlrpc_c::value_int(response);
    return retFile;
}

int HooFSRPCClient::create(const char *path, int mode) {
    int ret = -1;

    try {
        value res;
        ourClient.call(serverURL, _create, "si", &res, path, mode);
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
    int ret = -1;
    
    try {
        value res;
        ourClient.call(serverURL, _unlink, "s", &res, path);
        ret = value_int(res);
    }
    catch (exception const& e) {
        cerr << "Client threw error: " << e.what() << endl;
    } catch (...) {
        cerr << "Client threw unexpected error." << endl;
    }
}

int HooFSRPCClient::release(int fd) {
    /*
    try {
        ourClient.call(serverURL, _release, "s", &res, path);
        ret = value_int(res);
    }
    catch (exception const& e) {
        cerr << "Client threw error: " << e.what() << endl;
    } catch (...) {
        cerr << "Client threw unexpected error." << endl;
    }*/
     
}

struct stat *HooFSRPCClient::getAttr(const char *path, struct stat *stbuf) {

}

int HooFSRPCClient::rmdir(const char *path) {

}

int HooFSRPCClient::read(int fd, int size, int offset) {
    xmlrpc_c::value response;
    ourClient.call(serverURL, _read, "iii", &response, fd, size, offset);

    string retFile = xmlrpc_c::value_string(response);
    return retFile;
}

int HooFSRPCClient::write(int fd, const char *buf, int size, int offset) {

}

