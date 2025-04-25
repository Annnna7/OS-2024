#include "client.h"
using namespace System;  //подключение директив
using namespace System::Windows::Forms;

[STAThreadAttribute]      // атрибут указывает, что основной поток приложения должен быть 
                          //однопоточным (STA - Single-Threaded Apartment), что является требованием для использования Windows Forms
void main(array<String^>^ args)
{
    Application::EnableVisualStyles();  // подключаем визуальные стили
    Application::SetCompatibleTextRenderingDefault(false);   // для корректной обработки текста
    My65client::client form;  //Создается экземпляр класса client из пространства имен My65client
    Application::Run(% form);   //Запускает цикл обработки сообщений для указанной формы
}
