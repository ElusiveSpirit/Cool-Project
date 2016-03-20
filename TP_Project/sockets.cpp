#include <string.h>
#include "NetException.h"
#include "sockets.h"

#define ADDR_ANY ((net::in_addr_t) 0x00000000)
#define MAXPENDING 1

union convert {
    int32_t i;
    uint32_t u;
};

Socket::Socket(char *ip, unsigned short port) {
    socket = net::socket(PF_INET, net::SOCK_STREAM, net::IPPROTO_TCP);
    if (socket < 0) throw NetException("create socket failed");

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = net::inet_addr(ip);
    address.sin_port = net::htons(port);

    connected = false;
    closed = false;
}

Socket::Socket(int socket, net::sockaddr_in address) {
    this->socket = socket;
    this->address = address;
    connected = true;
    closed = false;
}

Socket::~Socket() {
    close();
    connected = false;
}

void Socket::connect() {
    if (connected) return;
    if (net::connect(socket, (net::sockaddr *) &address, sizeof(address)) < 0)
        throw NetException("connect socket failed");

    int value = 1;
    if (net::setsockopt(socket, net::IPPROTO_TCP, TCP_NODELAY, (char *) &value, sizeof(int)) < 0)
        throw NetException("TCP_NODELAY failed");
    connected = true;
}

void Socket::close() {
    if (!closed && net::close(socket) < 0) throw NetException("close socket failed");
    closed = true;
}

void Socket::raw_read(size_t size, void *addr) {
    size_t pos = 0;
    long i = 0;
    while (pos != size) {
        i = net::recv(socket, (char *) addr + pos, size - pos, 0);
        if (i == -1) {
            throw NetException("recv failed");
        }
        pos += i;
    }
}

void Socket::raw_write(size_t size, void *addr) {
    size_t pos = 0;
    long i = 0;
    while (pos != size) {
        i = net::send(socket, (char *) addr + pos, size - pos, 0);
        if (i == -1) {
            throw NetException("send failed");
        }
        pos += i;
    }
}

void Socket::write_byte(char b) {
    if (!connected)throw NetException("not connected");
    raw_write(sizeof(char), &b);
}

void Socket::write_int(int32_t value) {
    if (!connected)throw NetException("not connected");
    convert c;
    c.i = value;
    uint32_t uvalue = net::htonl(c.u);
    raw_write(sizeof(uint32_t), &uvalue);
}

void Socket::write_array(char *array, size_t length) {
    if (!connected)throw NetException("not connected");
    raw_write(length, array);
}

void Socket::write_string(char *str) {
    write_int((int32_t) strlen(str));
    write_array(str, strlen(str));
}

char Socket::read_byte() {
    if (!connected)throw NetException("not connected");
    char value;
    raw_read(sizeof(char), &value);
    return value;
}

int32_t Socket::read_int() {
    if (!connected)throw NetException("not connected");
    convert c;
    uint32_t uvalue;
    raw_read(sizeof(uint32_t), &uvalue);
    c.u = net::ntohl(uvalue);
    return c.i;
}

char *Socket::read_array(size_t length) {
    if (!connected)throw NetException("not connected");
    char *buf = new char[length];
    raw_read(length, buf);
    return buf;
}

char *Socket::read_string() {
    size_t length = (size_t) read_int();
    char *str = new char[length + 1];
    raw_read(length, str);
    str[length] = 0;
    return str;
}


ServerSocket::ServerSocket(unsigned short port) {
    socket = net::socket(PF_INET, net::SOCK_STREAM, net::IPPROTO_TCP);
    if (socket < 0) throw NetException("create socket failed");

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = net::htons(port);
    address.sin_addr.s_addr = net::htonl(ADDR_ANY);

    if (net::bind(socket, (net::sockaddr *) &address, sizeof(address)) < 0)
        throw NetException("bind socket failed");

    if (net::listen(socket, MAXPENDING) < 0)
        throw NetException("listen socket failed");
}

ServerSocket::~ServerSocket() {
    close();
}

Socket ServerSocket::accept() {
    int clnt_sock;
    net::sockaddr_in clnt_address;
    net::socklen_t clnt_address_len;
    clnt_sock = net::accept(socket, (net::sockaddr *) &clnt_address, &clnt_address_len);
    if (clnt_sock < 0) throw NetException("accept socket failed");

    int value = 1;
    if (net::setsockopt(clnt_sock, net::IPPROTO_TCP, TCP_NODELAY, (char *) &value, sizeof(int)) < 0)
        throw NetException("TCP_NODELAY failed");
    return Socket(clnt_sock, clnt_address);
}

void ServerSocket::close() {
    if (!closed && net::close(socket) < 0) throw NetException("close socket failed");
    closed = true;
}