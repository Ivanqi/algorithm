#include <iostream>
#include <string>
#include "ConsistentHash.h"
using namespace std;

void showTime(ConsistentHash &csshash) {
    cout << csshash.showTime();
    cout << "-----------------------------" << endl;
}

int main() {

    ConsistentHash csshash;
    csshash.addNode("192.168.0.1:8080", 2);
    csshash.addNode("192.168.0.2:8859", 3);
	csshash.addNode("10.0.1.56:80", 2);
	csshash.addNode("10.0.1.57:80", 3);
    showTime(csshash);

    // 模拟一个客户端 
    string cliIp = "192.168.56.1:12345";
    string serIp = csshash.getServerName(cliIp);
	cout << cliIp << "\thash值：" << csshash.getHash(cliIp) << " -> " << serIp << endl;


    cliIp = "10.1.0.194:12345";
	serIp = csshash.getServerName(cliIp);
	cout << cliIp << "\thash值：" << csshash.getHash(cliIp) << " -> " << serIp << endl;
    
    return 0;
}