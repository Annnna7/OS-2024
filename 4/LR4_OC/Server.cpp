#include <iostream>
#include <iostream>
#include <ctime>
#include <thread>
#include <vector>
//1 шаг 
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") 
#pragma warning(disable: 4996) 

using namespace std;

int ReadInt() {
    int n;
    std::cout << "Enter the number of clients > 3: ";
    while (!(cin >> n) || (n < 3) || (cin.peek() != '\n') || n <= 0) {
        cin.clear();
        while (cin.get() != '\n');
        std::cout << "Incorrect data. Try again > 3: ";
    }
    return n;
}

std::vector<SOCKET> clientSockets;

void function(SOCKET clientSocket, int clientCount) {
    while (true) {
        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            cout << "Message from the client " << clientCount << ": " << buffer << endl;
        }
        else {
            cout << "The client " << clientCount << " is disconnected" << endl;
            if (closesocket(clientSocket) == SOCKET_ERROR)
                cerr << "Failed to terminate connection.\n Error code: " << WSAGetLastError();
            break;
        }
    }
}

int main() {
    WORD DLLVersion = MAKEWORD(2, 2);  // ¬ерси€ интерфейса Windows Sockets
    WSAData wsaData; //создаем структуру дл€ загрузки
    //2 шаг. ѕроверить, можно ли пользоватьс€ сокетами с помощью функции
    int Result = WSAStartup(DLLVersion, &wsaData);
    if (Result != 0) { // проверка подключени€ библиотеки
        std::cout << "WSAStartup failed with error: " << Result << endl;
        return 1;
    }

    //«аполн€ем информацию об адресе сокета
    SOCKADDR_IN addr;
    int sizeOfAddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8888);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //3 шаг. —оздание сокета
    SOCKET sListen = socket(AF_INET, //семейство протоколов
        SOCK_STREAM, //тип создаваемого сокета (использование потоковых сокетов)
        IPPROTO_TCP); //тип протокола (рекомендуетс€ 0, чтобы ос выбрала протокол)
    if (sListen == INVALID_SOCKET) {
        std::cout << "Socket failed with error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    int clientCount = ReadInt();
    //соединение сервера с локальным адресом
    if (bind(sListen, //св€зываемый сокет
        (SOCKADDR*)&addr, //структура
        sizeof(addr)) == SOCKET_ERROR) //длина структуры в байтах
    {
        std::cout << "Error bind %d" << WSAGetLastError() << endl;;
        closesocket(sListen);
        WSACleanup();
        return 1;
    }

    //переводим сокет в режим ожидани€
    if (listen(sListen,
        SOMAXCONN) == SOCKET_ERROR) {//количество сообщений в очереди
        std::cout << "Listening failed with error: " << WSAGetLastError() << endl;
        closesocket(sListen);
        WSACleanup();
        return 1;
    }
    std::cout << "Waiting for client to connect..." << endl;

    for (int i = 1; i <= clientCount; i++) {
        // «апускаем .exe файл
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        if (CreateProcess(L"C:\\Users\\Asus\\Desktop\\24 инф\\LR4_OC_2\\x64\\Debug\\LR4_OC_2.exe",
            NULL, //аргумент командной строки
            NULL, //дескриптор процесса не наследуетс€
            NULL,  //дескриптор потока не наследуетс€
            FALSE,  //будут ли новому процессу наследоватьс€ дескрипторы
            CREATE_NEW_CONSOLE, //создает новое окно консоли дл€ нового процесса
            NULL,  //переменные среду окружени€
            NULL,  //полный путь к текущему рабочему каталогу
            &si,//указатель на STARTUPINFO
            &pi)) {  //указатель на PROCESS_INFORMATION

            //5 шаг. »звлечь запросы на подключение к сокету
            SOCKET clientSocket = accept(sListen,//дескриптор, который идентифицирует сокет
                nullptr, //необ€зательный указатель на буфер, который получает адрес подключающегос€ объекта
                nullptr); ///необ€зательный указатель на целое число, которое содержит длину структуры, на которую указывает параметр addr
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Accept failed.\\n";
                closesocket(sListen);
                WSACleanup();
                return 1;
            }
            clientSockets.push_back(clientSocket);
            char msg[256];
            sprintf(msg, "%d", i);
            //передаем сообщение о номере клиента
            send(clientSocket, //дескриптор, идентифицирующий подключенный сокет
                msg, //указатель на буфер, содержащий данные дл€ передачи
                sizeof(msg), NULL);
            std::cout << "New client " << i << " connected!" << endl;

            char m[256];
            recv(clientSocket, m, sizeof(m), 0);
            int a = atoi(m);

            thread t(function, clientSocket, a);
            t.join();
        }
        else cout << "Error creating process " << i << endl;
    }

    /*for (int i = 0; i < clientSockets.size(); i++) {
        char m[256];
        recv(clientSockets[i], m, sizeof(m), 0);
        int a = atoi(m);

        thread t(function, clientSockets[a-1], a);
        t.join();
        closesocket(clientSockets[a-1]);
    }*/

    std::cout << "Server stop working and close connection";
    //закрываем сокет и разрываем соединение
    if (closesocket(sListen) == SOCKET_ERROR)
        cerr << "Failed to terminate connection.\n Error code: " << WSAGetLastError();
    //отключение библиотеки WinSock2 и освобождени€ ресурсов перед закрытием программы
    WSACleanup();
    return 0;
}