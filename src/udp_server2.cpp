#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;

int main(int argc, char** argv) {

    const int server_port = atoi(argv[1]);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("error creating socket");
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(server_port);

    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("error binding to socket");
    }

    // Perhaps you should reset them before each iteration
    struct sockaddr_in from;
    unsigned int from_len = sizeof(struct sockaddr_in);
    char buffer[4096];
    string str;
    for (int i = 0; i < 10000000; ++i) {
        memset(&buffer, 0, sizeof(buffer));

        int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &from, &from_len);

        if (bytes < 0) {
            perror("error reading from socket");
        }

        cout << buffer << endl;

        memset(&buffer, 0, sizeof(buffer));
        getline(cin, str);
        strcpy(buffer,str.c_str());

        int sent_bytes = sendto(sock, buffer, bytes, 0, (struct sockaddr *) &from, sizeof(from));
        if (sent_bytes < 0) {
            perror("error writing to socket");
        }
        cout << "msg sent: " << buffer << endl;

    }

    close(sock);

    return 0;
}