#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include <string>
using namespace std;
#define PORT 1234
#define BUF_SIZE 64

int main() {
    // Инициализируем Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cout << "WSA failed\n";
        return 1;
    }

    // Создаём сокет
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cout << "Socket failed\n";
        WSACleanup();
        return 1;
    }

    // Говорим, КУДА подключаться (сервер: localhost:1234)
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // IP: localhost

    // Подключаемся к серверу
    if (connect(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        cout << "Connect failed\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    cout << "Connected to server!\n";
    char buf[BUF_SIZE];
    string msg;
    // Основной цикл: получаем вопрос → вводим ответ
    while (true) {
        int bytes = recv(sock, buf, BUF_SIZE - 1, 0);
        if (bytes <= 0) break;  // Сервер отключился
        buf[bytes] = '\0';
        cout << buf;  // Печатаем вопрос
        getline(cin, msg);
        send(sock, msg.c_str(), msg.size(), 0);

        if (msg == "Bye") break;
    }
    // Завершаем работу
    closesocket(sock);
    WSACleanup();
    return 0;
}