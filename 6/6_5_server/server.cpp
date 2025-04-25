#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#pragma comment(lib, "ws2_32.lib")
#include<thread>
#include<winsock2.h>
#include <locale>
#pragma warning(disable: 4996)//��� inet_addr

using namespace std;
int n;
ofstream outputFile("log.txt", ios::app);
vector <SOCKET> clientSocket(n);
HANDLE hMutex;

class Car {
public:

    string brand;
    string model;
    string year;
    string country;
    string price;
    string color;
    Car(string brand, string model, string year, string country, string price, string color) {

        this->brand = brand;
        this->model = model;
        this->year = year;
        this->country = country;
        this->price = price;
        this->color = color;

    }
};

vector<Car> Database = {
    Car("Tayota","Camry","2019","Japan","1000000","Blue"),
    Car("Porsche","956","1984","Germany","1100000","Black"),
    Car("BMW","M8","2019","Germany","1200000","Green"),
    Car("Audi","A4","1994","Germany","1200000","Grey"),
    Car("Hyundai","Solaris","2001","Korea","1200000","White"),
    Car("Kia","Rio","2005","Korea","1300000","Black"),
    Car("Audi","Q5","2008","Germany","1400000","Black"),
    Car("Lada","Vesta","2015","Russia","1450000","White"),
};


string getData(string name_characteristic, string characteristic) {


    string msg_foundCar = "";
    if (name_characteristic == "year") {

        for (Car car : Database) {

            if (characteristic == car.year) {
                msg_foundCar += "���������� ������: " + car.brand + " " + car.model + " " + car.year + " " + car.country + " " + car.price + " " + car.color + "\n";
            }
        }
    }
    else if (name_characteristic == "price") {

        for (Car car : Database) {

            if (characteristic == car.price) {
                msg_foundCar += "���������� ������: " + car.brand + " " + car.model + " " + car.year + " " + car.country + " " + car.price + " " + car.color + "\n";
            }
        }
    }
    else if (name_characteristic == "brand") {
        for (Car car : Database) {
            if (characteristic == car.brand) {
                msg_foundCar += "���������� ������: " + car.brand + " " + car.model + " " + car.year + " " + car.country + " " + car.price + " " + car.color + "\n";
            }
        }
    }
    else if (name_characteristic == "model") {
        for (Car car : Database) {
            if (characteristic == car.model)
            {
                msg_foundCar += "���������� ������: " + car.brand + " " + car.model + " " + car.year + " " + car.country + " " + car.price + " " + car.color + "\n";
            }
        }
    }
    else if (name_characteristic == "country") {
        for (Car car : Database) {
            if (characteristic == car.country) {
                msg_foundCar += "���������� ������: " + car.brand + " " + car.model + " " + car.year + " " + car.country + " " + car.price + " " + car.color + "\n";
            }
        }
    }
    else if (name_characteristic == "color") {
        for (Car car : Database) {

            if (characteristic == car.color)
            {
                msg_foundCar += "���������� ������: " + car.brand + " " + car.model + " " + car.year + " " + car.country + " " + car.price + " " + car.color + '\n';
            }
        }
    }
    else {
        msg_foundCar += "���������� �� ������!";
    }
    return msg_foundCar;
}
void changeData(string brand, string model, string year, string country, string price, string color) {

    Database.push_back(Car(brand, model, year, country, price, color));


}
void function(int i) {


    bool flag = true;

    do {
        char buffer[1024];
        int bytesReceived = recv(clientSocket[i], buffer, sizeof(buffer), 0);


        if (bytesReceived > 0) {
            string data(buffer, bytesReceived);

            // �������� ������� �����
            std::time_t currentTime = std::time(nullptr);
            // ����������� � ������
            char* timeString = std::ctime(&currentTime);
            outputFile << "Request client " << i << ": " << data << " [" << timeString << "]" << endl;

            stringstream ss(data);
            vector<string> words;
            string word;
            while (ss >> word) {
                words.push_back(word);
            }
            while (!ss.eof() && isspace(ss.peek())) {
                ss.get();
            }
            WaitForSingleObject(hMutex, INFINITE);
            if (words[0] == "getData") {

                string buf = getData(words[1], words[2]);
                if (buf == "") {
                    buf = "���������� �� ������!\n";
                }
                char msg_buffer[1024];
                strcpy(msg_buffer, buf.c_str());
                int bytesSent = send(clientSocket[i], msg_buffer, sizeof(msg_buffer), 0);

                // �������� ������� �����
                std::time_t currentTime = std::time(nullptr);
                // ����������� � ������
                char* timeString = std::ctime(&currentTime);
                outputFile << "Answer server " << buf << " [" << timeString << "]" << endl;

                if (bytesSent == -1) {
                    cerr << "������ �������� ������\n";
                }

            }
            else if (words[0] == "changeData") {

                changeData(words[1], words[2], words[3], words[4], words[5], words[6]);


                string buf = "Database updated!";
                char msg_buffer[1024];
                strcpy(msg_buffer, buf.c_str());

                for (int j = 0; j < clientSocket.size(); ++j) {
                    int bytesSent = send(clientSocket[j], msg_buffer, sizeof(msg_buffer), 0);
                }

                // �������� ������� �����
                std::time_t currentTime = std::time(nullptr);
                // ����������� � ������
                char* timeString = std::ctime(&currentTime);
                outputFile << "Answer server " << buf << " [" << timeString << "]" << endl;

            }
            else if (words[0] == "finish") {
                flag = false;
                string msg_buffer = "Bye";
                cout << msg_buffer;
                closesocket(clientSocket[i]);
                // �������� ������� �����
                std::time_t currentTime = std::time(nullptr);
                // ����������� � ������
                char* timeString = std::ctime(&currentTime);
                outputFile << "Client " << i << " disconnect" << " [" << timeString << "]" << endl;


            }
            else {
                char msg_buffer[1024];
                string buf = "������ ������� ������� ��� ��������� ������(finish)!";
                strcpy(msg_buffer, buf.c_str());
                int bytesSent = send(clientSocket[i], msg_buffer, sizeof(msg_buffer), 0);
                if (bytesSent == -1) {
                    cerr << "������ �������� ������\n";
                }

            }
        }
        else {
            cout << "������ �������� ���������" << endl;
            if (closesocket(clientSocket[i] == SOCKET_ERROR))
                cerr << "�� ������� �������� ����������.\n ��� ������: " << WSAGetLastError();

        }


    } while (flag);

    ReleaseMutex(hMutex);

}

int main()
{
    setlocale(LC_ALL, "Russian");
    outputFile << "-----------------------------------------------------------------" << endl;
    outputFile << "New session: " << endl;
    hMutex = CreateMutexW(NULL, FALSE, L"MyMutex");
    //�������� ���������� 
    WSAData wsaData; //������� ��������� ��� �������� 
    WORD DLLVersion = MAKEWORD(2, 1); // ������ ���������� winsock 
    if (WSAStartup(DLLVersion, &wsaData) != 0) { // �������� ����������� 
        cerr << "Error: failed to link library.\n";
        return 1;
    }
    //��������� ���������� �� ������ ������ 
    SOCKADDR_IN addr;
    int sizeOfAddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;
    SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0); //����� ��� ������������� �����,��������� �������� ����������,�������� 

    if (bind(sListen, (SOCKADDR*)&addr, sizeOfAddr) == SOCKET_ERROR) {
        //�������� ������ ������ 

        printf("Error bind %d\n", WSAGetLastError());
        if (closesocket(sListen) == SOCKET_ERROR)
            cerr << "Failed to terminate connection.\n Error code: " << WSAGetLastError();
        WSACleanup();
        return 1;
    }
    //����������� ������������� � ������������ �������� 
    if (listen(sListen, SOMAXCONN) == SOCKET_ERROR) {
        cout << "Listen failed;\n";
        if (closesocket(sListen) == SOCKET_ERROR)
            cerr << "Failed to terminate connection.\n Error code: " << WSAGetLastError();
        WSACleanup();
        return 1;
    }

    //������ ���������� ��� ���� �������� 
    STARTUPINFOW si;//��������� 
    PROCESS_INFORMATION pi;// ��������� � ����������� � �������� 
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);// ��������� ������ 
    ZeroMemory(&pi, sizeof(pi));
    DWORD dwThreadId = 0;
    //������ ����� ���� ��� �������� 
    cout << "Input n: ";
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        if (!CreateProcessW(
            L"C:\\Users\\Asus\\Desktop\\24 ���\\6_5_client\\x64\\Debug\\6_5_client.exe", // module name 
            NULL, // Command line (������ ������) 
            NULL, // Process handle not inheritable 
            NULL, // Thread handle not inheritable 
            FALSE, // Set handle inheritance to FALSE 
            CREATE_NEW_CONSOLE, //creation flags 
            NULL, // Use parent's environment block 
            NULL, // Use parent's starting directory 
            &si, // Pointer to STARTUPINFO structure 
            &pi // Pointer to PROCESS_INFORMATION structure 
        )) {
            cout << GetLastError();
        }
        Sleep(10);
        outputFile << "Create process-client: " << i << endl;
        clientSocket.push_back(accept(sListen, (SOCKADDR*)&addr, &sizeOfAddr));
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)function, (LPVOID)(i), NULL, NULL);

    }
    system("pause");

    if (closesocket(sListen) == SOCKET_ERROR) {
        cerr << "�� ������� �������� ����������.\n ��� ������: " << WSAGetLastError();
    }

    WSACleanup();
    return 0;
}