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
#include <cstring>
#include <cstdlib>
using namespace std;

//XMLRPC-C includes
//#include <xmlrpccpp.h>
#include "HooFSRPCClient.h"

value_struct statToXML(struct stat *myStat) {
    // Make the map value 'structData'
    map<std::string, xmlrpc_c::value> structData;
    pair<std::string, xmlrpc_c::value> ino("st_ino", value_int((int)myStat->st_ino));
    pair<std::string, xmlrpc_c::value> size("st_size", value_int((int)myStat->st_size));
    pair<std::string, xmlrpc_c::value> dev("st_dev", value_int((int)myStat->st_dev));
    pair<std::string, xmlrpc_c::value> mode("st_mode", value_int((int)myStat->st_mode));
    pair<std::string, xmlrpc_c::value> nlink("st_nlink", value_int((int)myStat->st_nlink));
    pair<std::string, xmlrpc_c::value> uid("st_uid", value_int((int)myStat->st_uid));
    pair<std::string, xmlrpc_c::value> gid("st_gid", value_int((int)myStat->st_gid));
    
    structData.insert(ino);
    structData.insert(size);
    structData.insert(dev);
    structData.insert(mode);
    structData.insert(nlink);
    structData.insert(uid);
    structData.insert(gid);
    
    // Make an XML-RPC struct out of it
    xmlrpc_c::value_struct param1(structData);
    return param1;
}

struct stat *XMLToStat(value_struct stat) {
    struct stat toRet;
    std::map<std::string, xmlrpc_c::value> info = stat.cvalue();
    int ino = value_int(info.at(_ino));
    int size = value_int(info.at(_size));
    int dev = value_int(info.at(_dev));
    int mode = value_int(info.at(_mode));
    int nlink = value_int(info.at(_nlink));
    int uid = value_int(info.at(_uid));
    int gid = value_int(info.at(_gid));
    
    toRet.st_ino = ino;
    toRet.st_size = size;
    toRet.st_dev = dev;
    toRet.st_mode = mode;
    toRet.st_nlink = nlink;
    toRet.st_uid = uid;
    toRet.st_gid = gid;
    
    return &toRet;
}

HooFSRPCClient::HooFSRPCClient(string ip, int serverPort) {
    serverURL = "http://" + ip + ":" + to_string(serverPort) + "/RPC2";
}

HooFSRPCClient::~HooFSRPCClient() {
    
}

char *HooFSRPCClient::readdir(const char *path) {
    string ret;
    //string s = path;
    //Read a directory listing
    try {
        xmlrpc_c::value response;
        ourClient.call(serverURL, _readdir, "s", &response, path);
        //cout << xmlrpc_c::value_int(response) << endl;
        ret = xmlrpc_c::value_string(response);
    }
    catch (exception const& e) {
        cerr << "Client threw error: " << e.what() << endl;
    } catch (...) {
        cerr << "Client threw unexpected error." << endl;
    }
   
    char str[ret.size()];
    strncpy(str, ret.c_str(), ret.size());
    
    return str;
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
        //value_struct statBuf = statToXML(stbuf);
        //ourClient.call(serverURL, _getAttr, "sS", &response, path, *stbuf);
        //ourClient.call(serverURL, _getAttr, "sS", &response, path, &stbuf);
        paramList params;
        params.add(value_string(path));
        params.add(statToXML(stbuf));
     
        ourClient.call(serverURL, _getAttr, params, &response);
        
        value_struct res(response);
        
        ret = XMLToStat(res);
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

int HooFSRPCClient::chmod(const char *path, int mode) {

    //Response of the chmod call for success/fail
    int ret = -1;

    //Call the server to chmod file
    try {
        value res;
        ourClient.call(serverURL, _chmod, "si", &res, path, mode);
        ret = value_int(res);
    }
    catch (exception const& e) {
        cerr << "Client threw error: " << e.what() << endl;
    } catch (...) {
        cerr << "Client threw unexpected error." << endl;
    }

    return ret;
}

int HooFSRPCClient::chown(const char *path, int uid, int gid) {

    //Response of the chown call for success/fail
    int ret = -1;

    //Call the server to chown file
    try {
        value res;
        ourClient.call(serverURL, _chown, "sii", &res, path, uid, gid);
        ret = value_int(res);
    }
    catch (exception const& e) {
        cerr << "Client threw error: " << e.what() << endl;
    } catch (...) {
        cerr << "Client threw unexpected error." << endl;
    }

    return ret;
}

int HooFSRPCClient::utime(const char *path, struct stat *uBuf) {

    //Response of the utime call for success/fail
    int ret = -1;

    //Call the server to utime file
    try {
        value res;
        ourClient.call(serverURL, _utime, "sS", &res, path, uBuf);
        ret = value_int(res);
    }
    catch (exception const& e) {
        cerr << "Client threw error: " << e.what() << endl;
    } catch (...) {
        cerr << "Client threw unexpected error." << endl;
    }

    return ret;
}

int HooFSRPCClient::trunc(const char *path, int newSize) {

    //Response of the truncate call for success/fail
    int ret = -1;

    //Call the server to truncate file
    try {
        value res;
        ourClient.call(serverURL, _truncate, "si", &res, path, newSize);
        ret = value_int(res);
    }
    catch (exception const& e) {
        cerr << "Client threw error: " << e.what() << endl;
    } catch (...) {
        cerr << "Client threw unexpected error." << endl;
    }

    return ret;
}

int HooFSRPCClient::rename(const char *path, const char *newPath) {
    //Response of the rename call for success/fail
    int ret = -1;

    //Call the server to truncate file
    try {
        value res;
        ourClient.call(serverURL, _rename, "ss", &res, path, newPath);
        ret = value_int(res);
    }
    catch (exception const& e) {
        cerr << "Client threw error: " << e.what() << endl;
    } catch (...) {
        cerr << "Client threw unexpected error." << endl;
    }

    return ret;
}
