#include "client.h"
using namespace System;  //����������� ��������
using namespace System::Windows::Forms;

[STAThreadAttribute]      // ������� ���������, ��� �������� ����� ���������� ������ ���� 
                          //������������ (STA - Single-Threaded Apartment), ��� �������� ����������� ��� ������������� Windows Forms
void main(array<String^>^ args)
{
    Application::EnableVisualStyles();  // ���������� ���������� �����
    Application::SetCompatibleTextRenderingDefault(false);   // ��� ���������� ��������� ������
    My65client::client form;  //��������� ��������� ������ client �� ������������ ���� My65client
    Application::Run(% form);   //��������� ���� ��������� ��������� ��� ��������� �����
}
