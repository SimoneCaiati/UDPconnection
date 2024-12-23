#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include "header.h"
#include "ClientLib.h"

void clearwinsock() {
#if defined WIN32
    WSACleanup();
#endif
}

int main() {
#if defined WIN32
    WSADATA wsadata;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (iResult != 0) {
        printf("Error at WSAStartup\n");
        return EXIT_FAILURE;
    }
#endif

    int sock;
    struct sockaddr_in s_Addr, from_Addr;
    int fromSize;
    struct hostent* host;
    char buffer[BUFFSIZE];

    // Hostname resolution
    if ((host = gethostbyname(HOSTNAME)) == NULL) {
        errorhandler("Name resolution error");
        return EXIT_FAILURE;
    }

    // Socket creation
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        errorhandler("socket() failed");
        clearwinsock();
        return EXIT_FAILURE;
    }

    // Set socket timeout
    struct timeval timeout;
    timeout.tv_sec = 5;  // Timeout di 5 secondi
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

    // Server address creation
    memset(&s_Addr, 0, sizeof(s_Addr));
    s_Addr.sin_family = AF_INET;
    s_Addr.sin_port = htons(PROTOPORT);
    memcpy(&s_Addr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);  // Hostname resolved in IP address

    // Input string
    char input[10];
    do {
        menu();
        char errorstring[50];
        printf("\nChoose the message:\n");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';  // Remove trailing newline

        if (msgValidate(input, errorstring)) {
            printf("Correct insertion...\n");

            // Sending the message to the server
            int msgLength = strlen(input);
            if (sendto(sock, input, msgLength, 0, (struct sockaddr*)&s_Addr, sizeof(s_Addr)) != msgLength) {
                errorhandler("sendto() sent a different number of bytes than expected");
                break;
            }
            printf("Message sent correctly to the server...\n");

            // Receive the password
            fromSize = sizeof(from_Addr);
            int receivedBytes = recvfrom(sock, buffer, BUFFSIZE - 1, 0, (struct sockaddr*)&from_Addr, &fromSize);

            if (receivedBytes < 0) {
                errorhandler("recvfrom() failed or timeout occurred");
                break;
            }

            buffer[receivedBytes] = '\0';  // Null-terminate the string
            if (s_Addr.sin_addr.s_addr != from_Addr.sin_addr.s_addr) {
                fprintf(stderr, "Error: Received a packet from an unknown source.\n");
                exit(EXIT_FAILURE);
            }
            printf("Received Password from Server: %s\n", buffer);

        } else {
            printf("%s...\n\n", errorstring);
        }
    } while (input[0] != 'q');

    closesocket(sock);
    clearwinsock();
    return EXIT_SUCCESS;
}
