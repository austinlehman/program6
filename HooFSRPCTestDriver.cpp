//
//  main.cpp
//  p6
//
//  Created by Austin Lehman on 11/14/22.
//

#include <iostream>
#include "HooFSRPCClient.h"
#include <fcntl.h>


int main(int argc, const char * argv[]) {
    HooFSRPCClient client("localhost", 8180);
    client.create("hi.txt", 0777);
    int fd = client.open("hi.txt", O_RDWR);
    client.write(fd, 3, 0, "hi\n");
    //client.unlink("hi.txt");
    return 0;
}
