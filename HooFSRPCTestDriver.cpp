/************************************************
*
* Author: Joshua Hunter and Austin Lehman
* Assignment: Program 6
* Class: CSI 4337
*
************************************************/

#include <iostream>
#include "HooFSRPCClient.h"
#include <fcntl.h>


//Constants
const int NUM_ARGS = 2;
const int PROG_ARG = 0;
const int IP_ARG = 1;
const int PORT_ARG = 2;
const int PORTMIN = 0;
const int PORTMAX = 65535;

int main(int argc, const char * argv[]) {

    /* Check Number of ARGS */
    if (argc - 1 != NUM_ARGS) {
        fprintf(stderr, "Usage: %s <server name/ip> <server port>\n", argv[PROG_ARG]);
        exit(1);
    }

    /* Validate server?????? */                     //FIX OR VALIDATE
    string serverIP = argv[IP_ARG];

    /* Validate port */
    int serverPort;
    sscanf(argv[PORT_ARG], "%d", &serverPort);
    if(serverPort < PORTMIN || serverPort > PORTMAX) {
        fprintf(stderr, "ERROR: Server port %d invalid. Port must be in the range 0 - 65535\n", serverPort);
        exit(3);
    }

    HooFSRPCClient client(serverIP, serverPort);
    client.create("hi.txt", 02);
    client.unlink("hi.txt");
    HooFSRPCClient client("localhost", 8180);
    client.create("hi.txt", 0777);
    int fd = client.open("hi.txt", O_RDWR);
    client.write(fd, 3, 0, "hi\n");
    //client.unlink("hi.txt");
    return 0;
}
