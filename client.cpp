#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <netinet/tcp.h>
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <sys/time.h>


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
    int getMessage( char ** );
    int sendMessage( char * );
private:


    void die( const char * );

};

TCPClient::TCPClient( char *IP, unsigned short port) :
    servIP (IP),
    echoServPort (port)
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

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        die("connect() failed");

    if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&value, sizeof(int)) < 0)
        die("TCP_NODELAY failed");

    return 0;
}

int TCPClient::sendMessage(char *message) {

    size_t messageSize = STRLEN(message) * sizeof(char);
    printf("%s = %lli\n", message, messageSize);

    if (send(sock, (char*)&messageSize, sizeof(size_t), 0) != sizeof(size_t)) {
        die("send() failed to send message");
    }

    if (send(sock, message, messageSize, 0) != messageSize) {
        die("send() failed to send message");
    }

    return 0;
}

void TCPClient::die(const char *errorMessage)
{
  perror(errorMessage);
  exit(1);
}




int main(int argc, char *argv[])
{
    TCPClient client("127.0.0.1", 8080);

    client.connectToServer();

    printf("Connected.\n");
    printf("Enter the message:\n");

    char *message = new char[50];
    scanf("%s\n", message);
    client.sendMessage(message);

    delete [] message;

    return 0;
}
