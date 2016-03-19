#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <netinet/tcp.h>
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <sys/time.h>
#include "chars.h"
#include "net.cpp"

class TCPClient {

    #define STRLEN(s) ({int retval = 0; while (*(s + ++retval)); retval;})


    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    int iterations = 2, gap = 1, i;          /* Number of timestamps to send, and gap between each send */
    struct timeval ts;
    long long c_ts, o_ts, delta, total = 0, max = 0, min = 1000000000;
    int value = 1;

public:

    TCPClient();
    TCPClient( char *, unsigned short );
    ~TCPClient(){
        if (sock)
            close(sock);
    };

    int connectToServer();
    char* receiveMessage();
    void sendMessage(char *);
private:

    int printError(const char *);
    void die( const char * );

};

int TCPClient::printError(const char *errorMessage)
{
    perror(errorMessage);
    return -1;
}

TCPClient::TCPClient( char *IP, unsigned short port) :
        echoServPort (port),
        servIP (IP)

{}


int TCPClient::connectToServer() {

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        die("socket() failed");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */

    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        die("connect() failed");

    if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&value, sizeof(int)) < 0)
        die("TCP_NODELAY failed");

    return 0;
}

char* TCPClient::receiveMessage() {
    return read_string(sock);
}

void TCPClient::sendMessage(char *msg) {
    write_string(sock, msg);
}

void TCPClient::die(const char *errorMessage)
{
  perror(errorMessage);
  exit(1);
}




int main(int argc, char **argv)
{
    char IP[] = "127.0.0.1";

    TCPClient client(IP, 8080);

    client.connectToServer();

    printf("Connected to server %s\n", IP);
    printf("Enter the message:\n");

    char *message = NULL;
    size_t size = 100;
    int len = 0;
    try {
        while (true) {
            printf("<- ");
            if ((len = getline(&message, &size, stdin)) == -1) break;
            if (len > 0 && message[len - 1] == '\n') {
                message[len - 1] = 0;
            }
            client.sendMessage(message);
            printf("-> %s\n", client.receiveMessage());
        }
    } catch (NetException n) {
        printf("net error");
    }

    delete [] message;
    return 0;
}
