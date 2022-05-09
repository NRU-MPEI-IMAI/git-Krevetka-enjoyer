
#define _CRT_SECURE_NO_WARNINGS

#include "stack"
#include "iostream"
#include "fstream"
#include "string"
#include <cmath>
#include "Aftomat.h"
#include <fcntl.h>
#include <io.h>
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cin;
using std::stack;



int main()
{
	setlocale(LC_ALL, "Russian");
	int b = 0;
	while (b != 4)
	{
		cout << "1.Ввод в ручную" << endl << "2.Ввод из файла" << endl << "Закончить" << endl;
		cin >> b;
		Aftomat a=Aftomat();
		if (b == 2)
		{
			cout << "Введите файл: ";
			string x;
			cin >> x;
			ifstream f;
			f.open(x);
			a=Aftomat(f) ;
			cout << endl;
		}
		else if (b==1)
		{
			cout << "Введите регулярное выражение: ";
			string x;
			cin >> x;
			a=Aftomat (x);
			cout << endl;
		}
		else
			break;
		cout << "1.Преобразование НКА в ДКА" << endl << "2.Прямое произведение" << endl << "Закончить"<<endl;
		cin >> b;
		if (b == 1)
		{
			cout << "Введите имя файла вывода: "<<endl;
			string x;
			cin >> x;
			a.AmazingNKAvDKA(x);
		}
		else if (b == 2)
		{
			cout << "1.Ввод в ручную" << endl << "2.Ввод из файла" << endl;
			cin >> b;
			Aftomat c = Aftomat();
			if (b == 2)
			{
				cout << "Введите файл: ";
				string x;
				cin >> x;
				ifstream f;
				f.open(x);
				c = Aftomat(f);
				cout << endl;
			}
			else
			{
				cout << "Введите регулярное выражение: ";
				string x;
				cin >> x;
				c = Aftomat(x);
				cout << endl;
			}
			cout << "1.Пересечение" << endl << "2.Объединение" << endl << "3.Разность" << endl << "Передумал"<<endl;
			cin >> b;
			cout << "Введите файл: "<<endl;
			string x;
			cin >> x;
			char w;
			if (b == 1)
				w = '*';
			else if (b == 2)
				w = '+';
			else if (b == 3)
				w = '-';
			else
				break; 
			a.AmazingProizvedenie(c, x, w);
		}
		else 
			break;
	}

	return 0;
}

