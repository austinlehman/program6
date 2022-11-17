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

    return nullptr;
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
    return ret;
}

int HooFSRPCClient::release(int fd) {
    int ret = -1;
    
    try {
        value res;
        ourClient.call(serverURL, _release, "i", &res, fd);
        ret = value_int(res);
    }
    catch (exception const& e) {
        cerr << "Client threw error: " << e.what() << endl;
    } catch (...) {
        cerr << "Client threw unexpected error." << endl;
    }
    
    return ret;
}


struct stat *HooFSRPCClient::getAttr(const char *path, struct stat *stbuf) {
    xmlrpc_c::value response;
    ourClient.call(serverURL, _getAttr, "sS", &response, path, stbuf);

    
    //This needs to be fixed
    return stbuf;
}
 

int HooFSRPCClient::rmdir(const char *path) {
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
    
    return ret;
}

string HooFSRPCClient::read(int fd, int size, int offset) {

    string retBuffer = "";

    try {
        xmlrpc_c::value response;
        ourClient.call(serverURL, _read, "iii", &response, fd, size, offset);
        retBuffer = (string) value_string(response);
    }
    catch (exception const& e) {
        cerr << "Client threw error: " << e.what() << endl;
    } catch (...) {
        cerr << "Client threw unexpected error." << endl;
    }

    return retBuffer;
}

int HooFSRPCClient::write(int fd, int size, int offset, const char *data) {

    int retBytesWritten = -1;

    try {
        xmlrpc_c::value response;
        ourClient.call(serverURL, _write, "isii", &response, fd, data, size, offset);
        retBytesWritten = xmlrpc_c::value_int(response);
    }
    catch (exception const& e) {
        cerr << "Client threw error: " << e.what() << endl;
    } catch (...) {
        cerr << "Client threw unexpected error." << endl;
    }

    return retBytesWritten;
}
