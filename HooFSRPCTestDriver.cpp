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
    client.create("hi.txt", 0777);
    int fd = client.open("hi.txt", O_RDWR);
    client.write(fd, 2, 0, "hi");
    client.release(fd);
    client.unlink("hi.txt");
    return 0;
}
