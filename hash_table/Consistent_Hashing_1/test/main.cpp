#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include "ConsistentHash.h"
#include <cctype>
using namespace std;

void printError() {
    cout << "input error !" << endl;
}

void printHelp() {
    const char *help_message = ""
		"Usage:"
		"\n\n add [host] [port] [num]"
		"\n     add server to container."
		"\n     num is virtual node number"
		"\n\n del [host] [port]"
		"\n     delte server to container."
		"\n\n get [host] [port]"
		"\n     get a server ip of container."
		"\n\n getVirNum [host] [port]"
		"\n     get the host virtual node number."
		"\n\n getRealNum"
		"\n     get all real server number."
		"\n\n getAllNodeNum"
		"\n     get all server number."
		"\n\n showTime"
		"\n     show the all key and value."
		"\n\n statDist"
		"\n     Statistics the distribution of all nodes"
		"\n\n h"
		"\n     Print this message."
		"\n\n q"
		"\n     Quit the application.\n";
    
    cout << help_message << endl;
}

int splitCMD(string cmd, const char *spl, vector<string> &v) {
    char *p = strtok((char *)cmd.c_str(), spl);
    int i = 0;
    while (p && i < 4) {
        v[i++] = string(p);
        p = strtok(NULL, spl);
    }

    return i;
}

int main(int argc, char const *argv[]) {

    if (argc != 1) {
        cout << "please input " << argv[0] << endl;
        return 0;
    }

    printHelp();
    string input;
    vector<string> v(4);
    ConsistentHash csshash;

    while (1) {
        cout << "<<<";
        getline(cin, input);
        if (input.empty()) {
            continue;
        }

        // 全部转化为小写(大小写不敏感)
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        int num = splitCMD(input, " ", v);
        cout << "v[0]: " << v[0] << " | num: " << num << endl;
        if (v[0] == "add") {
            if (num != 4) {
                printError();
            } else {
                csshash.addNode(v[1] + ":" + v[2], atoi(v[3].c_str()));
            }
        } else if (v[0] == "del") {
            if (num != 3) {
                printError();
            } else {
                csshash.delNode(v[1] + ":" + v[2]);
            }
        } else if (v[0] == "get") {
            if (num != 3) {
                printError();
            } else {
                cout << csshash.getServerName(v[1] + ":" + v[2]) << endl;;
            }
        } else if (v[0] == "getvirnum") {
            if (num != 3) {
                printError();
            } else {
                cout << csshash.getVirNum(v[1] + ":" + v[2]) << endl;
            }
        } else if (v[0] == "getrealnum") {
            if (num != -1) {
                printError();
            } else {
                cout << csshash.getRealNum() << endl;
            }
        } else if (v[0] == "getallnodenum") {
            if (num != -1) {
                printError();
            } else {
                cout << csshash.getAllNodeNum() << endl;
            }
        } else if (v[0] == "showtime") {
            if (num != -1) {
                printError();
            } else {
                cout << csshash.showTime();
            }
        } else if (v[0] == "statdist") {
            if (num != -1) {
                printError();
            } else {
                cout << csshash.statDist();
            }
        } else if (v[0] == "h") {
            if (num != -1) {
                printError();
            } else {
                return 0;
            }
        } else {
            printError();
        }
    }
    return 0;
}