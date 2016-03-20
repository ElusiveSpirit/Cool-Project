#include <iostream>
#include <cstring>
#include "sockets.h"

using namespace std;

int main() {
    ServerSocket ss(12345);
    Socket s = ss.accept();
    char *c;
    while (true) {
        c = s.read_string();
        cout << c << endl;
        s.write_string(c);
        if (!strcmp(c, "stop")) break;
        delete[] c;
    }
    delete[] c;
    s.close();
    ss.close();
    return 0;
}