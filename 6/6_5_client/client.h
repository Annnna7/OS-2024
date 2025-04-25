#include <iostream>
#include <thread>
#include <string>
#include <winsock2.h>
#include <msclr/marshal_cppstd.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996) // для inet_addr

namespace My65client {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Threading;

    public ref class client : public System::Windows::Forms::Form
    {
    public:
        client(void)
        {
            InitializeComponent();
            InitializeSocket();
            // Запуск потока для получения сообщений от сервера
            receiveThread = gcnew Thread(gcnew ThreadStart(this, &client::ReceiveMessages));
            receiveThread->IsBackground = true;
            receiveThread->Start();
        }

    protected:
        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        ~client()
        {
            if (components)
            {
                delete components;
            }

            // Закрытие сокета и очистка Winsock
            closesocket(Connection);
            WSACleanup();
            receiveThread->Abort();
        }

    private:
        HANDLE hMutex;
        System::Windows::Forms::ComboBox^ comboBox;
        System::Windows::Forms::TextBox^ valueTextBox;
    private: System::Windows::Forms::TextBox^ resTextBox1;


        System::Windows::Forms::Button^ getCarButton;
        System::Windows::Forms::TextBox^ newCarTextBox;
        System::Windows::Forms::TextBox^ addCarResponseTextBox;
        System::Windows::Forms::Button^ addCarButton;
        System::Windows::Forms::Button^ finishButton;
    private: System::ComponentModel::IContainer^ components;

           /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>

        SOCKET Connection;
    private: System::Windows::Forms::BindingSource^ bindingSource1;
           Thread^ receiveThread;

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        void InitializeComponent(void)
        {
            this->components = (gcnew System::ComponentModel::Container());
            this->comboBox = (gcnew System::Windows::Forms::ComboBox());
            this->valueTextBox = (gcnew System::Windows::Forms::TextBox());
            this->resTextBox1 = (gcnew System::Windows::Forms::TextBox());
            this->getCarButton = (gcnew System::Windows::Forms::Button());
            this->newCarTextBox = (gcnew System::Windows::Forms::TextBox());
            this->addCarResponseTextBox = (gcnew System::Windows::Forms::TextBox());
            this->addCarButton = (gcnew System::Windows::Forms::Button());
            this->finishButton = (gcnew System::Windows::Forms::Button());
            this->bindingSource1 = (gcnew System::Windows::Forms::BindingSource(this->components));
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->bindingSource1))->BeginInit();
            this->SuspendLayout();
            // 
            // comboBox
            // 
            this->comboBox->BackColor = System::Drawing::Color::LightGray;
            this->comboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(6) {
                L"brand", L"model", L"year", L"country", L"price",
                    L"color"
            });
            this->comboBox->Location = System::Drawing::Point(16, 15);
            this->comboBox->Margin = System::Windows::Forms::Padding(4);
            this->comboBox->Name = L"comboBox";
            this->comboBox->Size = System::Drawing::Size(345, 24);
            this->comboBox->TabIndex = 0;
            // 
            // valueTextBox
            // 
            this->valueTextBox->BackColor = System::Drawing::Color::Gray;
            this->valueTextBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->valueTextBox->Location = System::Drawing::Point(16, 48);
            this->valueTextBox->Margin = System::Windows::Forms::Padding(4);
            this->valueTextBox->Name = L"valueTextBox";
            this->valueTextBox->Size = System::Drawing::Size(345, 15);
            this->valueTextBox->TabIndex = 1;
            // 
            // resTextBox1
            // 
            this->resTextBox1->BackColor = System::Drawing::Color::Linen;
            this->resTextBox1->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->resTextBox1->Location = System::Drawing::Point(16, 80);
            this->resTextBox1->Margin = System::Windows::Forms::Padding(4);
            this->resTextBox1->Multiline = true;
            this->resTextBox1->Name = L"resTextBox1";
            this->resTextBox1->Size = System::Drawing::Size(345, 122);
            this->resTextBox1->TabIndex = 2;
            // 
            // getCarButton
            // 
            this->getCarButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)),
                static_cast<System::Int32>(static_cast<System::Byte>(192)));
            this->getCarButton->ForeColor = System::Drawing::SystemColors::ControlText;
            this->getCarButton->Location = System::Drawing::Point(16, 210);
            this->getCarButton->Margin = System::Windows::Forms::Padding(4);
            this->getCarButton->Name = L"getCarButton";
            this->getCarButton->Size = System::Drawing::Size(347, 28);
            this->getCarButton->TabIndex = 3;
            this->getCarButton->Text = L"Получить машину";
            this->getCarButton->UseVisualStyleBackColor = false;
            this->getCarButton->Click += gcnew System::EventHandler(this, &client::getCarButton_Click);
            // 
            // newCarTextBox
            // 
            this->newCarTextBox->BackColor = System::Drawing::Color::Gray;
            this->newCarTextBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->newCarTextBox->Location = System::Drawing::Point(16, 246);
            this->newCarTextBox->Margin = System::Windows::Forms::Padding(4);
            this->newCarTextBox->Name = L"newCarTextBox";
            this->newCarTextBox->Size = System::Drawing::Size(345, 15);
            this->newCarTextBox->TabIndex = 4;
            // 
            // addCarResponseTextBox
            // 
            this->addCarResponseTextBox->BackColor = System::Drawing::Color::Linen;
            this->addCarResponseTextBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->addCarResponseTextBox->Location = System::Drawing::Point(16, 278);
            this->addCarResponseTextBox->Margin = System::Windows::Forms::Padding(4);
            this->addCarResponseTextBox->Multiline = true;
            this->addCarResponseTextBox->Name = L"addCarResponseTextBox";
            this->addCarResponseTextBox->Size = System::Drawing::Size(345, 122);
            this->addCarResponseTextBox->TabIndex = 5;
            // 
            // addCarButton
            // 
            this->addCarButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)),
                static_cast<System::Int32>(static_cast<System::Byte>(192)));
            this->addCarButton->Location = System::Drawing::Point(16, 409);
            this->addCarButton->Margin = System::Windows::Forms::Padding(4);
            this->addCarButton->Name = L"addCarButton";
            this->addCarButton->Size = System::Drawing::Size(347, 28);
            this->addCarButton->TabIndex = 6;
            this->addCarButton->Text = L"Добавить в БД";
            this->addCarButton->UseVisualStyleBackColor = false;
            this->addCarButton->Click += gcnew System::EventHandler(this, &client::addCarButton_Click);
            // 
            // finishButton
            // 
            this->finishButton->BackColor = System::Drawing::Color::Red;
            this->finishButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->finishButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(204)));
            this->finishButton->Location = System::Drawing::Point(16, 475);
            this->finishButton->Margin = System::Windows::Forms::Padding(4);
            this->finishButton->Name = L"finishButton";
            this->finishButton->Size = System::Drawing::Size(347, 28);
            this->finishButton->TabIndex = 8;
            this->finishButton->Text = L"Завершить";
            this->finishButton->TextAlign = System::Drawing::ContentAlignment::TopCenter;
            this->finishButton->TextImageRelation = System::Windows::Forms::TextImageRelation::TextBeforeImage;
            this->finishButton->UseVisualStyleBackColor = false;
            this->finishButton->Click += gcnew System::EventHandler(this, &client::finishButton_Click);
            // 
            // client
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::Color::Black;
            this->ClientSize = System::Drawing::Size(368, 518);
            this->Controls->Add(this->finishButton);
            this->Controls->Add(this->addCarButton);
            this->Controls->Add(this->addCarResponseTextBox);
            this->Controls->Add(this->newCarTextBox);
            this->Controls->Add(this->getCarButton);
            this->Controls->Add(this->resTextBox1);
            this->Controls->Add(this->valueTextBox);
            this->Controls->Add(this->comboBox);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
            this->Margin = System::Windows::Forms::Padding(4);
            this->Name = L"client";
            this->Text = L"Client";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->bindingSource1))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }

        void InitializeSocket(void)
        {
            //Загрузка библиотеки
            WSAData wsaData; //создаем структуру для загрузки
            WORD DLLVersion = MAKEWORD(2, 1); // Версия библиотеки winsock
            if (WSAStartup(DLLVersion, &wsaData) != 0) { // проверка подключения
                MessageBox::Show("Ошибка инициализации Winsock", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
                exit(1);
            }

            // Настройка адреса сокета
            SOCKADDR_IN addr;
            addr.sin_addr.s_addr = inet_addr("127.0.0.1");
            addr.sin_port = htons(1111);
            addr.sin_family = AF_INET;

            // Создание сокета
            Connection = socket(AF_INET, SOCK_STREAM, 0);//сокет для прослушивания порта,семейство интернет протоколов,протокол

            //проверка на подключение к серверу
            if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
                int error_code = WSAGetLastError();
                String^ error_msg = "Не удалось подключиться к серверу. Код ошибки: " + error_code;
                MessageBox::Show(error_msg, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
                exit(1);
            }
        }

        void ReceiveMessages()
        {
            while (true)
            {
                char buffer[1024] = { 0 };
                int bytesReceived = recv(Connection, buffer, sizeof(buffer), 0);
                if (bytesReceived > 0)
                {
                    std::string str = std::string(buffer, 0, bytesReceived);
                    if (str == "Database updated!") {
                        System::String^ newMessage = gcnew System::String(str.c_str());
                        this->addCarResponseTextBox->AppendText(newMessage + Environment::NewLine);
                    }
                    else  this->resTextBox1->Text = gcnew System::String(str.c_str());
                }
                else   break;

            }
        }

    private:
        //кнопка найти машину по характеристике
        System::Void getCarButton_Click(System::Object^ sender, System::EventArgs^ e)
        {
            String^ parameter = this->comboBox->SelectedItem->ToString();
            String^ value = this->valueTextBox->Text;

            std::string parameterStr = msclr::interop::marshal_as<std::string>(parameter);
            std::string valueStr = msclr::interop::marshal_as<std::string>(value);
            std::string message = "getData" + std::string(" ") + parameterStr + std::string(" ") + valueStr;

            // Отправка сообщения на сервер
            int bytesSent = send(Connection, message.c_str(), (int)strlen(message.c_str()), 0);
            if (bytesSent == -1)
            {
                this->resTextBox1->Text = "Ошибка отправки данных на сервер";
                return;
            }
        }

        //кнопка добавить машину в БД
        System::Void addCarButton_Click(System::Object^ sender, System::EventArgs^ e)
        {
            String^ newCarData = this->newCarTextBox->Text;

            std::string newCarDataStr = msclr::interop::marshal_as<std::string>(newCarData);
            std::string message = "changeData " + newCarDataStr;

            // Отправка сообщения на сервер
            int bytesSent = send(Connection, message.c_str(), (int)strlen(message.c_str()), 0);
            if (bytesSent == -1)
            {
                this->addCarResponseTextBox->Text = "Ошибка отправки данных на сервер";
                return;
            }
        }

        //кнопка завершить работу клиента
        System::Void finishButton_Click(System::Object^ sender, System::EventArgs^ e)
        {
            // Отправка сообщения "finish" на сервер
            std::string finishMessage = "finish";
            int bytesSent = send(Connection, finishMessage.c_str(), finishMessage.length(), 0);
            if (bytesSent == SOCKET_ERROR)
            {
                MessageBox::Show("Ошибка отправки сообщения 'finish' на сервер", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }

            closesocket(Connection);
            WSACleanup();
            this->Close(); // Закрытие окна клиента
        }
};
}
