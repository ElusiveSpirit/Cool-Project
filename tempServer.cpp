#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <netinet/tcp.h>
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <stdio.h>  /* for perror() */
#include <stdlib.h> /* for exit() */
#include "chars.h"
#include "net.cpp"

class TCPServer {

    #define STRLEN(s) ({int retval = 0; while (*(s + ++retval)); retval;})
    #define MAXPENDING 1

    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned short echoServPort;     /* Server port */
    unsigned short quickAck;
    unsigned int clntLen;            /* Length of client address data structure */
    int value = 1;

public:

    TCPServer();
    TCPServer( unsigned short, unsigned short );
    ~TCPServer(){};

    void run();

    int init();
    char* receiveMessage();
    void sendMessage(char*);

private:

    void handle();
    // Printing the errorMessage and exit with 1
    void die( const char*);
    int printError( const char* );
};


int main(int argc, char **argv)
{

    TCPServer server(8080, 0);

    server.init();
    server.run();

    return 0;
}

void TCPServer::run() {
    for (;;) {
        /* Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr);

        printf("Waiting for client...\n");

        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
            die("accept() failed");

        /* clntSock is connected to a client! */

        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

        if (setsockopt(clntSock, IPPROTO_TCP, TCP_NODELAY, (char *)&value, sizeof(int)) < 0)
            die("TCP_NODELAY failed");

        char *buff = NULL;
        try {
            while (true) {
                buff = this->receiveMessage();
                printf("-> %s\n", buff);
                this->sendMessage(buff);
                delete[] buff;
            }
        } catch (NetException n) {
            printf("net error");
        }
    }
}

int TCPServer::init() {
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        return printError("socket() failed");

    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

    /* Bind to the local address */
    if (bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        return printError("bind() failed");

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        return printError("listen() failed");

    printf("Initialization finished\n");
    return 0;
}

void TCPServer::sendMessage(char *message) {
    write_string(clntSock, message);
}

char* TCPServer::receiveMessage() {
    return read_string(clntSock);
}

//void TCPServer::handle() {
//    long long c_ts;                     /* current read timestamp */
//    char *message = new char[50];
//    int value = 1;
//
//    if (quickAck && setsockopt(clntSock, IPPROTO_TCP, TCP_QUICKACK, (char *)&value, sizeof(int)) < 0)
//        die("TCP_QUICKACK failed");
//
//    size_t size = 0;
//    while (recv(clntSock, (char*)&size, sizeof(size_t), 0) == sizeof(size_t)) {
//
//        if (recv(clntSock, message, size, 0) == size) {
//            die("receive() failed");
//        }
//        printf("%s\n", message);
//
//        // Enable quickAck
//        if (quickAck && setsockopt(clntSock, IPPROTO_TCP, TCP_QUICKACK, (char *)&value, sizeof(int)) < 0)
//            die("TCP_QUICKACK failed");
//
//        /* Echo message back to client */
//        /*if (send(clntSock, (char*)&c_ts, sizeof(c_ts), 0) != sizeof(c_ts))
//            die("send() failed to send timestamp");*/
//
//        // Enable quickAck
//        if (quickAck && setsockopt(clntSock, IPPROTO_TCP, TCP_QUICKACK, (char *)&value, sizeof(int)) < 0)
//            die("TCP_QUICKACK failed");
//    }
//
//    delete [] message;
//    close(clntSock);    /* Close client socket */
//}

TCPServer::TCPServer( unsigned short echoServPort, unsigned short quickAck ) :
    echoServPort (echoServPort),
    quickAck (quickAck)
{}

int TCPServer::printError(const char *errorMessage)
{
    perror(errorMessage);
    return -1;
}

void TCPServer::die(const char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}
