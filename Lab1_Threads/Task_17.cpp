#include <iostream>
#include <cstdlib>
#include <string>
#include <Windows.h>

using namespace std;

const std::string IN_ERR_MSG = "Invalid input!";
const int MAX_IN = 10000;

HANDLE worker_mutex;

//Задача worker -- найти сумму всех чётных элементов массива.

DWORD WINAPI worker(LPVOID _params_ptr)
{
	int* vector = static_cast<int*>(_params_ptr);
	int arr_size = vector[0];
	for (int i = 2; i < arr_size + 2; i = i + 2)
	{
		WaitForSingleObject(worker_mutex, 0);
		vector[1] += vector[i];
		ReleaseMutex(worker_mutex);
		Sleep(12);
	}
	return 0;
}

bool safe_in (int& val, bool cycle, std::string msg)
{
	while (true) {
		cin >> val;
		if (cin.fail()) 
		{
			cin.clear();
			while (cin.get() != '\n');
			cout << msg << endl;
			if (!cycle)
				return false;
			continue;
		}
		break;
	}
	return true;
}

int main() 
{
	int size = 0;
	int* arr = nullptr;
	HANDLE hdl_worker;
	DWORD id_worker;
	worker_mutex = CreateMutex(NULL, false, NULL);
	cout << "Insert the size of the array:" << endl;
	while (true)
	{
		safe_in(size, true, IN_ERR_MSG);
		if (size > 1)
			break;
		cout << "Invalid size! Try again!" << endl;
	}
	arr = static_cast<int*>(malloc(sizeof(int) * (size + 2)));
	cout << "Insert elements of the array." << endl;
	for (int i = 0; i < size; i++)
	{
		int element = 0;
		safe_in(element, true, IN_ERR_MSG);
		arr[i + 2] = element;
	}
	arr[0] = size;
	arr[1] = 0; //Sum of even elements.
	hdl_worker = CreateThread(NULL, 0, worker, (void*)arr, 0, &id_worker);
	int* p_min = nullptr;
	int* p_max = nullptr;
	for (int i = 2; i < size + 2; i++) 
	{
		WaitForSingleObject(worker_mutex, 0);
		if ((p_min == nullptr) || (*p_min > arr[i])) {
			p_min = &(arr[i]);
		}
		ReleaseMutex(worker_mutex);
		Sleep(7);
		WaitForSingleObject(worker_mutex, 0);
		if ((p_max == nullptr) || (*p_max < arr[i])) {
			p_max = &(arr[i]);
		}
		ReleaseMutex(worker_mutex);
		Sleep(7);
	}
	WaitForSingleObject(hdl_worker, INFINITE);
	cout << "The biggest: " << *p_max << ", the smallest: " << *p_min << endl;
	cout << "Sum of even elements: " << arr[1] << endl;
	CloseHandle(hdl_worker);
	CloseHandle(worker_mutex);
	free(arr);
	system("pause");
	return 0;
}
