#include <stdio.h>
#include <winsock2.h>

int main()
{
    WSADATA wsa;
    SOCKET server_socket, client_socket;

    struct sockaddr_in server, client;
    int client_size;
    char buffer[1024];

    // initialising winsock which support functions up till version 2.2
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("WSA Startup failed . Error Code: %d\n", WSAGetLastError());
        return 0;
    }
    else
        printf("WinSock init successful !!");

    // IPv4,TCP,0
    // 0 means default protocol,
    // since we have mentioned SOCK_STREAM , it will use IPPROTO_TCP by default
    // that is why i have written socket(x,x,0)
    // we can also explicitly specify it
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET)
    {
        printf("Server Socket init failed . Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 0;
    }

    // specifies the address family to be used
    // AF_INET6 for IPv6; AF_INET for IPv4
    server.sin_family = AF_INET;

    // specifies the IP address socket will bind to
    // INADDR_ANY is a special constant means bind to all interfaces
    // this means server will accept incoming connections from any IP address assigned to machine
    server.sin_addr.s_addr = INADDR_ANY;

    // 8080 is in Host By Order
    // different computer architecture uses different byte orders
    // so it is necessary to convert port number to network byte order
    // since it is universal
    // it uses big endian order
    // htons -> Host to Network Short
    server.sin_port = htons(8080);

    // binds the server socket to server's IP
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Server socketing binding failed . Error Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 0;
    }
    printf("Server binding successful !!\n");

    if (listen(server_socket, 3) == SOCKET_ERROR)
    {
        printf("Listen Failed. Erro Code: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 0;
    }

    client_size = sizeof(client);

    // accepts the client connection to the server socket
    client_socket = accept(server_socket, (struct sockaddr *)&client, &client_size);

    if (client_socket == INVALID_SOCKET)
    {
        printf("Accept failed. \n");
        closesocket(server_socket);
        WSACleanup();
        return 0;
    }

    printf("Client connected to server !!\n");

    // recieve from client socket , some data ,size of data
    if (recv(client_socket, buffer, sizeof(buffer), 0) == SOCKET_ERROR)
    {
        printf("Receiving failed !!\n");
    }
    else
        printf("Client says: %s\n", buffer);

    char message[] = "Hello from server";

    // send to client_socket , some data ,size of data

    if (send(client_socket, message, sizeof(message), 0) == SOCKET_ERROR)
    {
        printf("Send failed\n");
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();
    return 0;
}