#include <iostream>
#include <string>
#include <winsock2.h>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

using namespace std;

int main()
{
    //Загрузка библиотеки
    WSAData wsaData; //создаем структуру для загрузки
    WORD DLLVersion = MAKEWORD(2, 1); //запрашиваемая версия библиотеки winsock
    int Result = WSAStartup(DLLVersion, &wsaData);

    if (Result != NO_ERROR) { //проверка подключения библиотеки
        cout << "WSAStartup failed with error: " << Result;
        return 1;
    }

    //сокет для соединения с сервером
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, NULL);
    //Заполняем информацию об адресе сокета
    SOCKADDR_IN addr;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //адрес
    addr.sin_port = htons(8888); //номер порта
    addr.sin_family = AF_INET; //семейство протоколов

    //4 шаг. Соединение с удаленным узлом
    if (connect(clientSocket, //подключаемый сокет
        (SOCKADDR*)&addr, //структура
        sizeof(addr)) == SOCKET_ERROR) { //длина структуры в байтах
        cout << "Failed to connect to server" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    char msg[256];
    recv(clientSocket, msg, sizeof(msg), NULL);  //считали с сервера номер клиента
    int a = atoi(msg);
    cout << "Hi! I am client " << a << "." << endl;
    char m[256];
    itoa(a, m, 10);
    send(clientSocket, m, sizeof(m), NULL);

    cout << "Connected!" << endl;
    string str;
    cin >> str;
    char msg_buffer[256];
    strcpy(msg_buffer, str.c_str());
    int bytesSent = send(clientSocket, msg_buffer, sizeof(msg_buffer), NULL);
    if (bytesSent == -1) {
        cerr << "Error sending data\n";
    }

    if (closesocket(clientSocket) == SOCKET_ERROR)
        cerr << "Failed to terminate connection.\n Error code: " << WSAGetLastError();
    WSACleanup();
    return 0;
}
