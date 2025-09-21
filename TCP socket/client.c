#include <stdio.h>
#include <winsock2.h>

int main()
{
    WSADATA wsa;
    SOCKET client_socket;

    struct sockaddr_in server;
    int client_size;
    char buffer[1024] = "Hello from client";

    // initialising winsock which support functions up till version 2.2
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("WSA Startup failed . Error Code: %d\n", WSAGetLastError());
        return 0;
    }
    else
        printf("WinSock init successful !!\n");

    // IPv4,TCP,0
    // 0 means default protocol,
    // since we have mentioned SOCK_STREAM , it will use IPPROTO_TCP by default
    // that is why i have written socket(x,x,0)
    // we can also explicitly specify it
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET)
    {
        printf("Server Socket init failed . Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }

    // specifies the address family to be used
    // AF_INET6 for IPv6; AF_INET for IPv4
    server.sin_family = AF_INET;

    // specifies the IP address socket will bind to
    // since we are building local socket communication
    // i will be using localhost i.e 127.0.0.1
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 8080 is in Host By Order
    // different computer architecture uses different byte orders
    // so it is necessary to convert port number to network byte order
    // since it is universal
    // it uses big endian order
    // htons -> Host to Network Short
    server.sin_port = htons(8080);

    // connection is running 127.0.0.1:8080

    // binds the server socket to server's IP
    if (connect(client_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Connection to server failed . Error Code: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 0;
    }
    printf("Client connected to server !!\n");

    // send to client_socket , some data ,size of data
    if (send(client_socket, buffer, strlen(buffer), 0) == SOCKET_ERROR)
    {
        printf("Send failed\n");
    }

    // recieve from client socket , some data ,size of data
    memset(buffer, 0, sizeof(buffer));
    if (recv(client_socket, buffer, sizeof(buffer), 0) == SOCKET_ERROR)
    {
        printf("Receiving failed !!\n");
    }
    else
        printf("Server says: %s\n", buffer);

    closesocket(client_socket);
    WSACleanup();
    return 0;
}
