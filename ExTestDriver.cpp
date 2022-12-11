/************************************************
 *
 * Author: Joshua Hunter and Austin Lehman
 * Assignment: Program 6 & 7
 * Class: CSI 4337
 *
 ************************************************/

//General Includes
#include <iostream>
#include <fcntl.h>
#include <sstream>
using namespace std;

//RPC Client Include
#include "HooFSRPCClient.h"

//Constants
const int NUM_ARGS = 2;
const int PROG_ARG = 0;
const int IP_ARG = 1;
const int PORT_ARG = 2;
const int PORTMIN = 0;
const int PORTMAX = 65535;

void fileInfo(HooFSRPCClient client, string fName) {
    string valid = "/" + fName;
    struct stat *fileStat = client.getAttr(valid.c_str());
    time_t createTime = fileStat->st_ctime;
    
    
    char buffer[26];
    struct tm *tm_info;
    tm_info = localtime(&createTime);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    
    cout << fName << "\t" << buffer << " ";
    printf( (S_ISDIR(fileStat->st_mode)) ? "d" : "-");
    printf( (fileStat->st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat->st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat->st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat->st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat->st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat->st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat->st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat->st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat->st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");
}

void listContents(HooFSRPCClient client, string path, bool isDir = true) {
    vector<string> files;
    
    cout << "Listing contents path: " << path << endl;
    
    if(isDir) {
        char* ls = client.readdir(path.c_str());
        
        cout << "List contents: ";
        if(strlen(ls) > 0) {
            
            stringstream ss(ls);
            string s;
            while (getline(ss, s, ' ')) {
                files.push_back(s);
            }
            
            for(int i = 0; i < files.size(); i++) {
                fileInfo(client, files.at(i));
            }
            cout << "SUCCESS"<< endl << endl;
        }
        else {
            cout << "FAILURE" << endl << endl;
        }
        
    }
    else {
        cout << "List contents: ";
        try {
            
            fileInfo(client, path);
            cout << "SUCCESS" << endl << endl;
        } catch(...) {
            cout << "FAILURE" << endl << endl;
        }
    }
}

int main(int argc, const char * argv[]) {
    if (argc - 1 != NUM_ARGS) {
        fprintf(stderr, "Usage: %s <server name/ip> <server port>\n", argv[PROG_ARG]);
        exit(1);
    }
    
    //Register server
    string serverIP = argv[IP_ARG];
    
    //Register and validate port
    int serverPort;
    sscanf(argv[PORT_ARG], "%d", &serverPort);
    if(serverPort < PORTMIN || serverPort > PORTMAX) {
        fprintf(stderr, "ERROR: Server port %d invalid. Port must be in the range 0 - 65535\n", serverPort);
        exit(2);
    }
    
    //Initialize client for RPC communication
    HooFSRPCClient client(serverIP, serverPort);
    
    
    cout << "Creating file1 for user read/write permissions" << endl;
    client.create("/file1", 0600);
    int fd = client.open("/file1", O_RDWR);
    cout << "Creation of file1: ";
    if(fd != -1) {
        cout << "SUCCESS with fd: " << fd << endl << endl;
    }
    else {
        cout << "FAILURE" << endl << endl;
    }
    
    
    cout << "Writing \"yap\" to file1" << endl;
    int wrote = client.write(fd, 4, 0, "yap\0");
    cout << "Write of \"yap\" to file1: ";
    if(wrote != 3) {
        cout << "SUCCESS with numBytes: " << wrote << endl << endl;
    }
    else {
        cout << "FAILURE" << endl << endl;
    }
    
    cout << "Closing file1" << endl;
    int closed = client.release(fd);
    cout << "Close file1: ";
    if(closed >= 0) {
        cout << "SUCCESS with value: " << closed << endl << endl;
    }
    else {
        cout << "FAILURE" << endl << endl;
    }
    
    
    cout << "Opening file1" << endl;
    fd = client.open("/file1", O_RDWR);
    cout << "Open file1: ";
    if(fd != -1) {
        cout << "SUCCESS with value: " << fd << endl << endl;
    }
    else {
        cout << "FAILURE" << endl << endl;
    }
    
    char buf[5];
    cout << "Reading file1" << endl;
    string str = client.read(fd, 4, 0);
    strncpy(buf, str.c_str(), 3);
    cout << "Read: ";
    if(buf[0] == 'y' && buf[1] == 'a' && buf[2] == 'p') {
        cout << "SUCCESS read: " << str << endl << endl;
    }
    else {
        cout << "FAILURE" << endl << endl;
    }
    
    cout << "Closing file1" << endl;
    closed = client.release(fd);
    cout << "Close file1: ";
    if(closed >= 0) {
        cout << "SUCCESS with value: " << closed << endl << endl;
    }
    else {
        cout << "FAILURE" << endl << endl;
    }
    
    cout << "Creating file2 for user read/write permissions for user and read only for group/world" << endl;
    client.create("/file2", 0644);
    fd = client.open("/file2", O_RDWR);
    cout << "Creation of file2: ";
    if(fd != -1) {
        cout << "SUCCESS with fd: " << fd << endl << endl;
    }
    else {
        cout << "FAILURE" << endl << endl;
    }
    
    cout << "Closing file2" << endl;
    closed = client.release(fd);
    cout << "Close file2: ";
    if(closed >= 0) {
        cout << "SUCCESS with value: " << closed << endl << endl;
    }
    else {
        cout << "FAILURE" << endl << endl;
    }
    
    listContents(client, "/");
    
    cout << "Changing permissions on file 2 to read/write -> user & group, and nothing for world" << endl;
    int success = client.chmod("/file2", 0440);
    if (success >= 0) {
        cout << "SUCCESS with value: " << success << endl << endl;
    }
    else {
        cout << "FAILURE" << endl << endl;
    }
    
    
    
    listContents(client, "/file2", false); //this one fails
    
    
    
    
    //MIDDLE STUFF
    
    
    cout << "Removing file fileStuff from directory stuff (Should fail because mkdir unimplemented)" << endl;
    
    success = client.unlink("/stuff/fileStuff");
    
    if (success >= 0) {
        cout << "SUCCESS with value: " << success << endl << endl;
    }
    else {
        cout << "FAILURE" << endl << endl;
    }
    cout << "Listing contents should fail because mkdir unimplemented by class" << endl;
    cout << "FAILURE" << endl;
    
    
    cout << "Removing stuff directory" << endl;
    
    success = client.rmdir("/stuff"); //this fails becuase we cannot create directories
    if (success >= 0) {
        cout << "SUCCESS with value: " << success << endl << endl;
    }
    else {
        cout << "FAILURE" << endl << endl;
    }
    cout << "Listing contents should fail because mkdir unimplemented by class" << endl;
    cout << "FAILURE" << endl;
    
    
    
    cout << "Removing file fileStuff from stuff directory (that does not exist)" << endl;
    success = client.unlink("/stuff/fileStuff");
    if (success < 0) {
        cout << "SUCCESS with failure to remove with value: " << success << endl << endl;
    }
    else {
        cout << "FAILURE" << endl << endl;
    }
    
    listContents(client, "/");
    
    cout << "Unable to create directory \"thing\" because mkdir not implemented by class" << endl << endl;
    cout << "Unable to list directory \"thing\" because mkdir not implemented by class" << endl << endl;
    
}
