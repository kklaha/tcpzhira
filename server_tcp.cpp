#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>  // ВАЖНО: без этого getline может не работать!
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define PORT 1234
#define BUF_SIZE 64

int main() {
    //  Инициализация Winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cout << "WSA failed\n";
        return 1;
    }

    //  Создаём сокет
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        cout << "Socket failed\n";
        WSACleanup();
        return 1;
    }

    //  Настраиваем свой адрес (на каком порту слушать)
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;  // Принимать с любого интерфейса
    addr.sin_port = htons(PORT);
    if (bind(s, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "Bind failed\n";
        closesocket(s);
        WSACleanup();
        return 1;
    }
    if (listen(s, 1) == SOCKET_ERROR) {
        cout << "Listen failed\n";
        closesocket(s);
        WSACleanup();
        return 1;
    }
    cout << "Server is listening on port " << PORT << "...\n";
    sockaddr_in client;
    int client_len = sizeof(client);
    SOCKET client_sock = accept(s, (sockaddr*)&client, &client_len);
    if (client_sock == INVALID_SOCKET) {
        cout << "Accept failed\n";
        closesocket(s);
        WSACleanup();
        return 1;
    }
    cout << "Client connected!\n";
    char buf[BUF_SIZE];
    string response;
    //  Основной цикл общения
    while (true) {
        const char* question = "Who are you? ";
        send(client_sock, question, strlen(question), 0);

        int bytes = recv(client_sock, buf, BUF_SIZE - 1, 0);
        if (bytes <= 0) break;  // Клиент отключился или ошибка
        buf[bytes] = '\0';  // Завершаем строку нулём

        cout << "Client said: " << buf << endl;

        if (string(buf) == "Bye") break;

        cout << "You (server): ";
        getline(cin, response);  // Считываем ответ от оператора сервера
    }
    cout << "Goodbye!\n";
    // Закрываем всё
    closesocket(client_sock);   // Закрываем соединение с клиентом
    closesocket(s);             // Закрываем основной сокет сервера
    WSACleanup();               // Завершаем работу с Winsock

    return 0;
}