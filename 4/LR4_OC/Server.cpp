#include <iostream>
#include <iostream>
#include <ctime>
#include <thread>
#include <vector>
//1 ��� 
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
    WORD DLLVersion = MAKEWORD(2, 2);  // ������ ���������� Windows Sockets
    WSAData wsaData; //������� ��������� ��� ��������
    //2 ���. ���������, ����� �� ������������ �������� � ������� �������
    int Result = WSAStartup(DLLVersion, &wsaData);
    if (Result != 0) { // �������� ����������� ����������
        std::cout << "WSAStartup failed with error: " << Result << endl;
        return 1;
    }

    //��������� ���������� �� ������ ������
    SOCKADDR_IN addr;
    int sizeOfAddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8888);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //3 ���. �������� ������
    SOCKET sListen = socket(AF_INET, //��������� ����������
        SOCK_STREAM, //��� ������������ ������ (������������� ��������� �������)
        IPPROTO_TCP); //��� ��������� (������������� 0, ����� �� ������� ��������)
    if (sListen == INVALID_SOCKET) {
        std::cout << "Socket failed with error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    int clientCount = ReadInt();
    //���������� ������� � ��������� �������
    if (bind(sListen, //����������� �����
        (SOCKADDR*)&addr, //���������
        sizeof(addr)) == SOCKET_ERROR) //����� ��������� � ������
    {
        std::cout << "Error bind %d" << WSAGetLastError() << endl;;
        closesocket(sListen);
        WSACleanup();
        return 1;
    }

    //��������� ����� � ����� ��������
    if (listen(sListen,
        SOMAXCONN) == SOCKET_ERROR) {//���������� ��������� � �������
        std::cout << "Listening failed with error: " << WSAGetLastError() << endl;
        closesocket(sListen);
        WSACleanup();
        return 1;
    }
    std::cout << "Waiting for client to connect..." << endl;

    for (int i = 1; i <= clientCount; i++) {
        // ��������� .exe ����
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        if (CreateProcess(L"C:\\Users\\Asus\\Desktop\\24 ���\\LR4_OC_2\\x64\\Debug\\LR4_OC_2.exe",
            NULL, //�������� ��������� ������
            NULL, //���������� �������� �� �����������
            NULL,  //���������� ������ �� �����������
            FALSE,  //����� �� ������ �������� ������������� �����������
            CREATE_NEW_CONSOLE, //������� ����� ���� ������� ��� ������ ��������
            NULL,  //���������� ����� ���������
            NULL,  //������ ���� � �������� �������� ��������
            &si,//��������� �� STARTUPINFO
            &pi)) {  //��������� �� PROCESS_INFORMATION

            //5 ���. ������� ������� �� ����������� � ������
            SOCKET clientSocket = accept(sListen,//����������, ������� �������������� �����
                nullptr, //�������������� ��������� �� �����, ������� �������� ����� ��������������� �������
                nullptr); ///�������������� ��������� �� ����� �����, ������� �������� ����� ���������, �� ������� ��������� �������� addr
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Accept failed.\\n";
                closesocket(sListen);
                WSACleanup();
                return 1;
            }
            clientSockets.push_back(clientSocket);
            char msg[256];
            sprintf(msg, "%d", i);
            //�������� ��������� � ������ �������
            send(clientSocket, //����������, ���������������� ������������ �����
                msg, //��������� �� �����, ���������� ������ ��� ��������
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
    //��������� ����� � ��������� ����������
    if (closesocket(sListen) == SOCKET_ERROR)
        cerr << "Failed to terminate connection.\n Error code: " << WSAGetLastError();
    //���������� ���������� WinSock2 � ������������ �������� ����� ��������� ���������
    WSACleanup();
    return 0;
}