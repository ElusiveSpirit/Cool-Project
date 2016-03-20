#ifndef SOCKETS_H
#define SOCKETS_H

#include <stdint-gcc.h>
#include <stddef.h>

namespace net {
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unistd.h>
}

class Socket {

    int socket;
    net::sockaddr_in address;
    bool connected;
    bool closed;

public:
    Socket(int socket, net::sockaddr_in address);

    Socket(char *ip, unsigned short port);

    ~Socket();

    void connect();

    void close();

    void write_byte(char b);

    void write_int(int32_t value);

    void write_array(char *array, size_t length);

    void write_string(char *str);

    char read_byte();

    int32_t read_int();

    char *read_array(size_t length);

    char *read_string();

private:
    void raw_read(size_t size, void *addr);

    void raw_write(size_t size, void *addr);
};

class ServerSocket {
    int socket;
    net::sockaddr_in address;
    bool closed;

public:
    ServerSocket(unsigned short port);

    ~ServerSocket();

    Socket accept();

    void close();
};

#endif //SOCKETS_H
