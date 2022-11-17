#include "HooFSRPCClient.h"

HooFSRPCClient::HooFSRPCClient(int cPort, int serverPort, string serverIP) {
    try {
        string const serverURL("http://" + serverIP + ":" + serverPort + "/RPC2");
    }
}
