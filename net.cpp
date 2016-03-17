#include <stddef.h>
#include <sys/socket.h>
#include <string.h>

class NetException {} nex;

void raw_read(int socket, size_t size, void *addr) {
    size_t pos = 0;
    int i = 0;
    while (pos != size) {
        i = recv(socket, (char*)addr + pos, size - pos, 0);
        if (i == -1) {
            throw nex;
        }
        pos += i;
    }
}

void raw_send(int socket, size_t size, void *addr) {
    size_t pos = 0;
    int i = 0;
    while (pos != size) {
        i = send(socket, (char*)addr + pos, size - pos, 0);
        if (i == -1) {
            throw nex;
        }
        pos += i;
    }
}

template <typename T>
T read_primitive(int socket) {
    T value;
    raw_read(socket, sizeof(T), &value);
    return value;
}

char* read_string(int socket) {
    size_t len = 0;
    raw_read(socket, sizeof(size_t), &len);
    char* str = new char[len + 1];
    raw_read(socket, len, str);
    str[len] = 0;
    return str;
}

template <typename T>
void write_primitive(int socket, T value) {
    raw_send(socket, sizeof(T), &value);
}

void write_string(int socket, char *str) {
    raw_send(socket, strlen(str), str);
}

int main() {}