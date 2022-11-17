//
//  main.cpp
//  p6
//
//  Created by Austin Lehman on 11/14/22.
//

#include <iostream>
#include "HooFSRPCClient.h"

int main(int argc, const char * argv[]) {
    HooFSRPCClient client("localhost", 8180);
    client.create("hi.txt", 02);
    client.unlink("hi.txt");
    return 0;
}
