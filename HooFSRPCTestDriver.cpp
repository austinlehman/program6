/************************************************
*
* Author: Joshua Hunter and Austin Lehman
* Assignment: Program 6
* Class: CSI 4337
*
************************************************/

//General Includes
#include <iostream>
#include <fcntl.h>
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

int main(int argc, const char * argv[]) {

    //Check Number of arguments
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

    //Testing RPC Calls
    cout << "creating hi.txt with all permissions" << endl;
    client.create("hi.txt", 0777);
    cout << "created hi.txt successfully" << endl << endl;
    
    cout << "calling readdir on ./" << endl;
    cout << client.readdir("./") << endl << endl;
    
    cout << "calling getattr on hi.txt" << endl;
    struct stat *stbuf;
    client.getAttr("hi.txt", stbuf);
    cout << "Size: " << stbuf->st_size << " (should be 0)" << endl;

    
    cout << "opening hi.txt for reading and writing" << endl;
    int fd = client.open("hi.txt", O_RDWR);
    cout << "opened hi.txt successfully" << endl << endl;
    
    cout << "writing \"hi\" to hi.txt" << endl;
    client.write(fd, 3, 0, "hi\0");
    cout << "wrote \"hi\" to hi.txt successfully" << endl << endl;
    
    cout << "reading from hi.txt" << endl;
    string s = client.read(fd, 3, 0);
    cout << "read " << s << " from hi.txt" << endl << endl;

    cout << "releasing the file descriptor for hi.txt" << endl;
    client.release(fd);
    cout << "released file descriptor successfully" << endl << endl;
    
    cout << "unlinking (removing) hi.txt" << endl;
    client.unlink("hi.txt");
    cout << "hi.txt unlinked successfully" << endl << endl;
    
    return 0;
}
