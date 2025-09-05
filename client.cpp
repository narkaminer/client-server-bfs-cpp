#include "std_lib_facilities.h"
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc != 5) {
    cerr << "Usage: " << argv[0] << " <ip> <port> <v1> <v2>" << endl;
    return 1;
}

    string ip = argv[1];
    int port = atoi(argv[2]);
    int v1 = atoi(argv[3]);
    int v2 = atoi(argv[4]);

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
    connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    string request = to_string(v1) + " " + to_string(v2);
    send(clientSocket, request.c_str(), request.length(), 0);

    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0) error("Error receiving response from server");
    buffer[bytesRead] = '\0';
    cout << "Shortest path: " << buffer << endl;

    close(clientSocket);
}