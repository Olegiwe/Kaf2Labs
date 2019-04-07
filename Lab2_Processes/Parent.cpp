#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

struct Student 
{
	int num;
	char name[32];
	double grade;
};

//Подлежат замене на свои
const char FILEPATH[] = "D:\\Perl\\students.txt";
const char CHILDPATH[] = "D:\\Perl\\Child.exe";

int main()
{
	fstream file(FILEPATH, std::fstream::out | std::fstream::in | std::fstream::trunc);
	Student student;
	char fileline[100];
	char args[64];
	if (!file.is_open()) 
	{
		cout << "File opening error!" << endl;
		system("pause");
		return -2;
	}
	int students_number = 0;
	cout << "Insert the number of students" << endl;
	cin >> students_number;
	if (cin.fail())
	{
		cout << "Invalid input. Fatal error!" << endl;
		system("pause");
		return -1;
	}
	for (int i = 0; i < students_number; i++)
	{
		cout << "Insert student's #" << i << " data:" << endl;
		cin >> student.num;
		cin >> student.name;
		cin >> student.grade;
		if (cin.fail())
		{
			cout << "Invalid input. Fatal error!" << endl;
			return -1;
		}
		file << student.num << "\t" << student.name << "\t" << student.grade << endl;
	}
	file.seekp(file.beg);
	cout << "Old file:" << endl;
	while (file.getline(fileline, 100))
		cout << fileline << endl;
	file.close();
	cout << "Insert id and name of a student and a new grade." << endl;
	cin >> student.num;
	cin >> student.name;
	cin >> student.grade;
	if (cin.fail())
	{
		cout << "Invalid input. Fatal error!" << endl;
		system("pause");
		return -1;
	}
	sprintf(args, "%s %d %s %e", FILEPATH, student.num, student.name, student.grade);
	STARTUPINFO stinf;
	ZeroMemory(&stinf, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pinf;
	if (!CreateProcess(CHILDPATH, args, NULL, NULL, false, CREATE_NEW_CONSOLE, NULL, NULL, &stinf, &pinf))
	{
		cout << "Process creation error!" << endl;
		system("pause");
		return -3;
	}
	WaitForSingleObject(pinf.hProcess, INFINITE);
	ifstream rfile(FILEPATH);
	cout << "New file:" << endl;
	while (rfile.getline(fileline, 100))
		cout << fileline << endl;
	rfile.close();
	CloseHandle(pinf.hThread);
	CloseHandle(pinf.hProcess);
	system("pause");
	return 0;
}
