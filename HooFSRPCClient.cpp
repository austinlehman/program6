/************************************************
 *
 * Author: Joshua Hunter and Austin Lehman
 * Assignment: Program 6
 * Class: CSI 4337
 *
 ************************************************/

//General includes
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

//XMLRPC-C includes
//#include <xmlrpccpp.h>
#include "HooFSRPCClient.h"

HooFSRPCClient::HooFSRPCClient(string ip, int serverPort) {
    serverURL = "http://" + ip + ":" + to_string(serverPort) + "/RPC2";
    
}

dirent *HooFSRPCClient::readdir(const char *path) {

    //Read a directory listing
    try {
        xmlrpc_c::value response;
        ourClient.call(serverURL, _readdir, "", &response);
        cout << xmlrpc_c::value_int(response) << endl;
    }
    catch (exception const& e) {
        cerr << "Client threw error: " << e.what() << endl;
    } catch (...) {
        cerr << "Client threw unexpected error." << endl;
    }
    
    return nullptr;
}

int HooFSRPCClient::open(const char *path, int flags) {

    //Response for the open file
    int retFile = -1;

    //Response of the open file call
    try {
        xmlrpc_c::value response;
        ourClient.call(serverURL, _open, "si", &response, path, flags);
        retFile = xmlrpc_c::value_int(response);
    }
    catch (exception const& e) {
        cerr << "Client threw error: " << e.what() << endl;
    } catch (...) {
        cerr << "Client threw unexpected error." << endl;
    }

    return retFile;
}

int HooFSRPCClient::create(const char *path, int mode) {

    //Response of the create call for success/fail
    int ret = -1;

    //Response of the create file call
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

    //Response of the unlink call for success/fail
    int ret = -1;

    //Call the server to unlink file
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

    //Response of the release call for success/fail
    int ret = -1;

    //Call the server to get a release file response
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

    //List of attributes to be returned
    struct stat *ret;

    //Call the server to get attributes
    try {
        xmlrpc_c::value response;
        ourClient.call(serverURL, _getAttr, "sS", &response, path, *stbuf);
        xmlrpc_c::value_struct res(response);
        ret = (struct stat *)(&res);
    }
    catch (exception const& e) {
        cerr << "Client threw error: " << e.what() << endl;
    } catch (...) {
        cerr << "Client threw unexpected error." << endl;
    }

    return ret;
}


int HooFSRPCClient::rmdir(const char *path) {

    //Response of the rmdir call for success/fail
    int ret = -1;

    //Call the server to get a unlink response
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

    //Data read from the read
    string retBuffer = "";

    //Call the server to get a read response
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

    //Number of bytes written
    int retBytesWritten = -1;

    //Call the server to get a write response
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