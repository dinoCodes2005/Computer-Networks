#include <stdio.h>
#include <winsock2.h>
#define PORT 8080

int main()
{
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server;
    char buffer[1024] = "Hello from client";
    int server_len = sizeof(server);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Winsock init failed. Error Code: %d\n", WSAGetLastError());
        return 0;
    }

    // Create UDP socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == INVALID_SOCKET)
    {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }

    // Setup server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // server IP

    // Send message to server
    if (sendto(client_socket, buffer, strlen(buffer), 0,
               (struct sockaddr *)&server, server_len) == SOCKET_ERROR)
    {
        printf("Send failed. Error Code: %d\n", WSAGetLastError());
    }

    // Receive response from server
    int recv_len = recvfrom(client_socket, buffer, sizeof(buffer), 0,
                            (struct sockaddr *)&server, &server_len);
    if (recv_len == SOCKET_ERROR)
    {
        printf("Receive failed. Error Code: %d\n", WSAGetLastError());
    }
    else
    {
        buffer[recv_len] = '\0'; // Null-terminate
        printf("Server says: %s\n", buffer);
    }

    // Cleanup
    closesocket(client_socket);
    WSACleanup();
    return 0;
}
