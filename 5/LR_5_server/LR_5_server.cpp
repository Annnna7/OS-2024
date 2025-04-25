#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#pragma comment(lib, "ws2_32.lib")
#include<thread>
#include<winsock2.h>
#pragma warning(disable: 4996)//для inet_addr

using namespace std;
int n;
vector <SOCKET> clientSocket(n);
HANDLE hMutex = CreateMutexW(NULL, FALSE, //вызывающий поток не получает права владения мьютексом
	L"MyMutex");

class Car {
	public:
		string brand;
		string model;
		string year;
		string country;
		string price;
		string color;
		Car(string brand, string model, string year, string country, string price, string color) {
			this->brand = brand; //обращение к полям класса 
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

string getData(string name, string characteristic) {
	string msg_foundCar = "";
	if (name == "year") {
		for (Car car : Database) {
			if (characteristic == car.year) {
				msg_foundCar += "Автомобиль найден: " + car.brand + " " + car.model + " " + car.year + " " + car.country + " " + car.price + " " + car.color + "\n";
			}
		}
	}
	else if (name == "price") {
		for (Car car : Database) {
			if (characteristic == car.price) {
				msg_foundCar += "Автомобиль найден: " + car.brand + " " + car.model + " " + car.year + " " + car.country + " " + car.price + " " + car.color + "\n";
			}
		}
	}
	else if (name == "brand") {
		for (Car car : Database) {
			if (characteristic == car.brand) {
				msg_foundCar += "Автомобиль найден: " + car.brand + " " + car.model + " " + car.year + " " + car.country + " " + car.price + " " + car.color + "\n";
			}
		}
	}
	else if (name == "model") {
		for (Car car : Database) {
			if (characteristic == car.model)
			{
				msg_foundCar += "Автомобиль найден: " + car.brand + " " + car.model + " " + car.year + " " + car.country + " " + car.price + " " + car.color + "\n";
			}
		}
	}
	else if (name == "country") {
		for (Car car : Database) {
			if (characteristic == car.country) {
				msg_foundCar += "Автомобиль найден: " + car.brand + " " + car.model + " " + car.year + " " + car.country + " " + car.price + " " + car.color + "\n";
			}
		}
	}
	else if (name == "color") {
		for (Car car : Database) {
			if (characteristic == car.color)
			{
				msg_foundCar += "Автомобиль найден: " + car.brand + " " + car.model + " " + car.year + " " + car.country + " " + car.price + " " + car.color + '\n';
			}
		}
	}
	else {
		msg_foundCar += "Автомобиль не найден!";
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
			string data(buffer, bytesReceived);  //Создание строки data из данных в буфере buffer, используя количество принятых байт
			stringstream ss(data);  //Создание потока строк ss из строки data
			vector<string> words;  // Объявление вектора строк words
			string word;
			while (ss >> word) {  //Чтение слов из потока
				words.push_back(word);
			}
			while (!ss.eof() && isspace(ss.peek())) {  //Пропуск пробелов после последнего слова
				ss.get();
			}
			if (words[0] == "getData") {
				string buf = getData(words[1], words[2]);
				if (buf == "") {
					buf = "Автомобиль не найден!\n";
				}
				char msg_buffer[1024];
				strcpy(msg_buffer, buf.c_str());  // Копирование содержимого строки buf в массив символов msg_buffer
				int bytesSent = send(clientSocket[i], msg_buffer, sizeof(msg_buffer), NULL);
				if (bytesSent == -1) {
					cerr << "Ошибка отправки данных\n";
				}
			}
			else if (words[0] == "changeData") {

				changeData(words[1], words[2], words[3], words[4], words[5], words[6]);

				string buf = "БД обновлена!\n";
				char msg_buffer[1024];
				strcpy(msg_buffer, buf.c_str());
				int bytesSent = send(clientSocket[i], msg_buffer, sizeof(msg_buffer), NULL);
				if (bytesSent == -1) {
					cerr << "Ошибка отправки данных\n";
				}
			}
			else if (words[0] == "finish") {
				flag = false;
				string msg_buffer = "До свидания!\n";
				cout << msg_buffer;
				int bytesSent = send(clientSocket[i], msg_buffer.c_str(), sizeof(msg_buffer), NULL);
				closesocket(clientSocket[i]);
			}
			else {
				char msg_buffer[1024];
				string buf = "Сервер ожидает запроса: ";
				strcpy(msg_buffer, buf.c_str());
				int bytesSent = send(clientSocket[i], msg_buffer, sizeof(msg_buffer), NULL);
				if (bytesSent == -1) {
					cerr << "Ошибка отправки данных\n";
				}
			}
		}
		else {
			cout << "Ошибка передачи сообщения" << endl;
			if (closesocket(clientSocket[i] == SOCKET_ERROR))
				cerr << "Не удалось прервать соединение.\n Код ошибки: " << WSAGetLastError();
		}
	} while (flag);
}

int main()
{
	setlocale(LC_ALL, "Russian");
	//Загрузка библиотеки
	WSAData wsaData; //создаем структуру для загрузки
	WORD DLLVersion = MAKEWORD(2, 1); // Версия библиотеки winsock
	if (WSAStartup(DLLVersion, &wsaData) != 0) { // проверка подключения
		cerr << "Error: failed to link library.\n";
		return 1;
	}
	//Заполняем информацию об адресе сокета
	SOCKADDR_IN addr;
	int sizeOfAddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0); //сокет для прослушивания порта,семейство интернет протоколов,протокол

	if (bind(sListen, (SOCKADDR*)&addr, sizeOfAddr) == SOCKET_ERROR) {
		//привязка адреса сокету

		printf("Error bind %d\n", WSAGetLastError());
		if (closesocket(sListen) == SOCKET_ERROR)
			cerr << "Failed to terminate connection.\n Error code: " << WSAGetLastError();
		WSACleanup();
		return 1;
	}
	//подключение прослушивания с максимальной очередью
	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR) {
		cout << "Listen failed;\n";
		if (closesocket(sListen) == SOCKET_ERROR)
			cerr << "Failed to terminate connection.\n Error code: " << WSAGetLastError();
		WSACleanup();
		return 1;
	}

	//задаем информацию для окна открытия
	STARTUPINFOW si;//структура
	PROCESS_INFORMATION pi;// структура с информацией о процессе
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);// указываем размер
	ZeroMemory(&pi, sizeof(pi));
	DWORD dwThreadId = 0;
	//создаём новые окна для клиентов
	cout << "Введите кол-во клиентов: ";
	cin >> n;
	for (uint8_t i = 0; i < n; i++)
	{
		if (!CreateProcessW(L"C:\\Users\\Asus\\Desktop\\24 инф\\LR_5_client\\x64\\Debug\\LR_5_client.exe", // module name
			NULL, //аргумент командной строки
			NULL, //дескриптор процесса не наследуется
			NULL,  //дескриптор потока не наследуется
			FALSE,  //будут ли новому процессу наследоваться дескрипторы
			CREATE_NEW_CONSOLE, //создает новое окно консоли для нового процесса
			NULL,  //переменные среду окружения
			NULL,  //полный путь к текущему рабочему каталогу
			&si,//указатель на STARTUPINFO
			&pi)) {  //указатель на PROCESS_INFORMATION
			cout << GetLastError();
		}
		Sleep(10);
		clientSocket.push_back(accept(sListen, (SOCKADDR*)&addr, nullptr));

		// возвращает дескриптор потока, который действителен только внутри нашего процесса
		CreateThread(NULL, //атрибуты защиты
			NULL, //  начальный размер стека для потока
			(LPTHREAD_START_ROUTINE)function, //  указатель на адрес функции, которая будет выполняться как отдельная задача
			(LPVOID)(i), // параметр, который будет передан внутрь функции потока
			NULL, // исполнение начинается немедленно
			NULL); // значения флагов, которые определяют поведение потока при его создании
	}
	system("pause");

	if (closesocket(sListen) == SOCKET_ERROR) {
		cerr << "The connection could not be terminated.\n Error code: " << WSAGetLastError();
	}

	WSACleanup();
	return 0;

}