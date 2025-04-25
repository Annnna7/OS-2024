#include <iostream>
#include <string>
#include <winsock2.h>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

using namespace std;

int main()
{
    //�������� ����������
    WSAData wsaData; //������� ��������� ��� ��������
    WORD DLLVersion = MAKEWORD(2, 1); //������������� ������ ���������� winsock
    int Result = WSAStartup(DLLVersion, &wsaData);

    if (Result != NO_ERROR) { //�������� ����������� ����������
        cout << "WSAStartup failed with error: " << Result;
        return 1;
    }

    //����� ��� ���������� � ��������
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, NULL);
    //��������� ���������� �� ������ ������
    SOCKADDR_IN addr;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //�����
    addr.sin_port = htons(8888); //����� �����
    addr.sin_family = AF_INET; //��������� ����������

    //4 ���. ���������� � ��������� �����
    if (connect(clientSocket, //������������ �����
        (SOCKADDR*)&addr, //���������
        sizeof(addr)) == SOCKET_ERROR) { //����� ��������� � ������
        cout << "Failed to connect to server" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    char msg[256];
    recv(clientSocket, msg, sizeof(msg), NULL);  //������� � ������� ����� �������
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
