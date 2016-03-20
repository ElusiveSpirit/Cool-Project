#ifndef NETEXCEPTION_H
#define NETEXCEPTION_H

#include <string>

using std::string;

class NetException {
    string message;
public:
    NetException(string s);
};

#endif //NETEXCEPTION_H
