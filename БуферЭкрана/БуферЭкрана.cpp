#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;

int BufScr()
{
	HANDLE hStdOutOld, hStdOutNew; // дескрипторы буфера экрана
	DWORD dwWritten; // для количества выведенных символов
	// создаем буфер экрана
	hStdOutNew = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE, // чтение и запись
		0, // не разделяемый
		NULL, // защита по умолчанию
		CONSOLE_TEXTMODE_BUFFER, // текстовый режим
		NULL); // не используется
	if (hStdOutNew == INVALID_HANDLE_VALUE)
	{
		_cputs("Create console screen buffer failed.\n");
		return GetLastError();
	}
	// сохраняем старый буфер экрана
	hStdOutOld = GetStdHandle(STD_OUTPUT_HANDLE);
	// ждем команду на переход к новому буферу экрана
	_cputs("Press any key to set new screen buffer active.\n");
	_getch();
	// делаем активным новый буфер экрана
	if (!SetConsoleActiveScreenBuffer(hStdOutNew))
	{
		_cputs("set new console active screen buffer failed.\n");
		return GetLastError();
	}
	// выводим текст в новый буфер экрана
	char text[] = "This is a new screen buffer.";
	if (!WriteConsole(
		hStdOutNew, // дескриптор буфера экрана
		text, // символы, которые выводим
		sizeof(text), // длина текста
		&dwWritten, // количество выведенных символов
		NULL)) // не используется
		_cputs("Write console output character failed.\n");
	// выводим сообщение о вводе символа
	char str[] = "\nPress any key to set old screen buffer.";
	if (!WriteConsole(
		hStdOutNew, // дескриптор буфера экрана
		str, // символы, которые выводим
		sizeof(str), // длина текста
		&dwWritten, // количество выведенных символов
		NULL)) // не используется
		_cputs("Write console output character failed.\n");
	_getch();
	// восстанавливаем старый буфер экрана
	if (!SetConsoleActiveScreenBuffer(hStdOutOld))
	{
		_cputs("set old console active screen buffer failed.\n");
		return GetLastError();
	}
	// пишем в старый буфер экрана
	_cputs("This is an old console screen buffer.\n");
	// закрываем новый буфер экрана
	CloseHandle(hStdOutNew);
	// ждем команду на завершение программы
	_cputs("Press any key to finish.\n");
	_getch();
	return 0;
}

int ButInform()
{
	HANDLE hStdOut; // дескриптор стандартного вывода
	CONSOLE_SCREEN_BUFFER_INFO csbi; // для параметров буфера экрана
	// читаем стандартный дескриптор вывода
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	// читаем параметры буфера экрана
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))
		std::cout << "Console screen buffer info failed." << std::endl;
	std::cout << "Console screen buffer info: " << std::endl << std::endl;
	// выводим на консоль параметры выходного буфера
	std::cout << "A number of columns= " << csbi.dwSize.X << std::endl;
	std::cout << "A number of rows = " << csbi.dwSize.Y << std::endl;
	std::cout << "X cursor coordinate = " << csbi.dwCursorPosition.X << std::endl;
	std::cout << "Y cursor coordinate = " << csbi.dwCursorPosition.Y << std::endl;
	std::cout << "Attributes = " << std::hex << csbi.wAttributes << std::dec << std::endl;
	std::cout << "Window upper corner = "
		<< csbi.srWindow.Left << ","
		<< csbi.srWindow.Top << std::endl;
	std::cout << "Window lower corner = "
		<< csbi.srWindow.Right << ","
		<< csbi.srWindow.Bottom << std::endl;
	std::cout << "Maximum number of columns = "
		<< csbi.dwMaximumWindowSize.X << std::endl;
	std::cout << "Maximum number of rows = "
		<< csbi.dwMaximumWindowSize.Y << std::endl << std::endl;
	return 0;
}

int BufSet()
{
	COORD coord; // для размера буфера экрана
	HANDLE hStdOut; // дескриптор стандартного вывода
	// читаем дескриптор стандартного вывода
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	// вводим новый размер буфера экрана
	std::cout << "Enter new screen buffer size." << std::endl;
	std::cout << "A number of columns: ";
	std::cin >> coord.X;
	std::cout << "A number of rows: ";
	std::cin >> coord.Y;
	// устанавливаем новый размер буфера экрана
	if (!SetConsoleScreenBufferSize(hStdOut, coord))
	{
		std::cout << "Set console screen buffer size failed." << std::endl;
		return GetLastError();
	}
	return 0;
}

int ReadWriteConsole()
{
	HANDLE hStdOut, hStdIn; // дескрипторы консоли
	DWORD dwWritten, dwRead; // для количества символов
	char buffer[80]; // для ввода символов
	char str[] = "Input any string:";
	char c;
	// читаем дескрипторы консоли
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE || hStdIn == INVALID_HANDLE_VALUE)
	{
		cout << "Get standard handle failed." << endl;
		return GetLastError();
	}
	// выводим сообщения о вводе строки
	if (!WriteConsole(hStdOut, &str, sizeof(str), &dwWritten, NULL))
	{
		cout << "Write console failed." << endl;
		return GetLastError();
	}
	// вводим строку
	if (!ReadConsole(hStdIn, &buffer, sizeof(buffer), &dwRead, NULL))
	{
		cout << "Read console failed." << endl;
		return GetLastError();
	}
	// ждем команду на завершение работы
	cout << "Input any char to exit: ";
	cin >> c;
	return 0;
}


int main() {
	bool exit = false;
	int choose;
	while (!exit) {
		cout << "Choose function!\n ";
		cout << "1: createActiveConsole \n 2: OutputFromBufferConsole \n 3: SetNewSizeOfBuffer \n 4: ReadAndWriteConsole\n 5:exit\n";
		cin >> choose;
		switch (choose) {
		case 1:
			if (BufScr() != 0) {
				cout << "Error occurred in createActiveConsole." << endl;
				return 1;
			}
			break;
		case 2:
			if (ButInform() != 0) {
				cout << "Error occurred in OutputFromBufferConsole." << endl;
				return 1;
			}
			break;
		case 3:
			if (BufSet() != 0) {
				cout << "Error occurred in SetNewSizeOfBuffer." << endl;
				return 1;
			}
			break;
		case 4:
			if (ReadWriteConsole() != 0) {
				cout << "Error occurred in ReadAndWriteConsole." << endl;
				return 1;
			}
			break;
		case 5:
			exit = true;
		}

	}
	return 0;
}


