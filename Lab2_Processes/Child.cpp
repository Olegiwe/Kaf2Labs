#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

int main(int argc, char** argv)
{
	cout << "Command line arguments: " << endl;
	for (int i = 0; i < argc; i++)
		cout << argv[i] << endl;
	if (argc < 4)
	{
		cout << "Not enough arguments!" << endl;
		system("pause");
		return -4;
	}
	fstream file(argv[0], std::fstream::in | std::fstream::out);
	if (!file.is_open())
	{
		cout << "File opening error!" << endl;
		system("pause");
		return -1;
	}
	std::string strmask;
	std::string strline;
	vector<std::string> fls;
	double grade = 0.0;
	try
	{
		grade = std::stod(argv[3]);
		sprintf(argv[3], "%.2lf", grade);

	}
	catch (const std::invalid_argument&)
	{
		cout << "Double parsing error!" << endl;
		system("pause");
		return -5;
	}
	while (std::getline(file, strline))
	{
		strmask = argv[2];
		if (strline.find(strmask) != std::string::npos) 
			strline = static_cast<std::string>(argv[1]) + "\t" + static_cast<std::string>(argv[2]) + "\t" + 
				static_cast<std::string>(argv[3]);
		fls.push_back(strline);
	}
	file.close();
		//for (auto it = fls.begin(); it != fls.end(); it++)
		//	file.write((*it + "\n").c_str(), 100);
	fstream tfile(argv[0], std::fstream::out | std::fstream::trunc);
	std::for_each(fls.begin(), fls.end(), [&](std::string& str) {
		tfile << str << endl;
	});
	tfile.close();
	system("pause");
	return 0;
}
