#include <iostream>
#include <cstring>
#include "sockets.h"

using namespace std;

int main() {
    char ip[] = "127.0.0.1";
    Socket s(ip, 12345);
    s.connect();
    char c[1000], *t;
    while (true) {
        cout << "< ";
        cin.getline(c, 1000);
        s.write_string(c);
        t = s.read_string();
        cout << "> " << t << endl;
        if (!strcmp(c, "stop")) break;
    }
    s.close();
    return 0;
}