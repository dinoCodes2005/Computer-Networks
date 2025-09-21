#include <stdio.h>
#include <winsock2.h>
#define PORT 8080

int main()
{
    WSADATA wsa;
    SOCKET server_socket;
    struct sockaddr_in server, client;
    char buffer[1024];
    int client_len = sizeof(client);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Winsock init failed. Error Code: %d\n", WSAGetLastError());
        return 0;
    }

    // Create UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == INVALID_SOCKET)
    {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }

    // Setup server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Server binding failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 0;
    }
    printf("Server binding successful!\n");

    // Receive message from client
    while (1)
    {

        int recv_len = recvfrom(server_socket, buffer, sizeof(buffer), 0,
                                (struct sockaddr *)&client, &client_len);
        if (recv_len == SOCKET_ERROR)
        {
            printf("Receiving failed !!\n");
        }
        else
        {
            buffer[recv_len] = '\0'; // Null-terminate string
            printf("Client says: %s\n", buffer);
        }

        // Send response to client
        char message[] = "Hello from server";
        if (sendto(server_socket, message, sizeof(message), 0,
                   (struct sockaddr *)&client, client_len) == SOCKET_ERROR)
        {
            printf("Send failed\n");
        }
    }

    // Cleanup
    closesocket(server_socket);
    WSACleanup();
    return 0;
}
