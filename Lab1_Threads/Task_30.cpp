#include <iostream>
#include <clocale>
#include <Windows.h>

using namespace std;

HANDLE mtx;

DWORD WINAPI worker(LPVOID params)
{
	int* arr = (int*)params;
	int size = arr[0];
	int* min = nullptr;
	for (int i = 2; i < size + 2; i++)
	{
		WaitForSingleObject(mtx, INFINITE);
		if (i % 2 == 0)
			arr[1] += arr[i];
		ReleaseMutex(mtx);
		Sleep(12);
	}
	return 0;
}

int main(int argc, char** argv) {
	setlocale(LC_ALL, "RU");
	HANDLE t_handle;
	DWORD t_id;
	mtx = CreateMutex(NULL, false, NULL);
	int arr_size = 0;
	cout << "Введите размер массива:" << endl;
	cin >> arr_size;
	int* arr = (int*)malloc(sizeof(int)*(2 + arr_size));
	arr[0] = arr_size;
	arr[1] = 0;
	cout << "Введите элементы массива:" << endl;
	for (int i = 2; i < arr_size + 2; i++)
		cin >> arr[i];
	t_handle = CreateThread(NULL, 0, worker, (void*)arr, 0, &t_id);
	int* p_min = nullptr;
	int* p_max = nullptr;
	for (int i = 2; i < arr_size + 2; i++)
	{
		WaitForSingleObject(mtx, INFINITE);
		if ((p_min == nullptr) || (*p_min > arr[i])) {
			p_min = &(arr[i]);
		}
		ReleaseMutex(mtx);
		Sleep(7);
		WaitForSingleObject(mtx, INFINITE);
		if ((p_max == nullptr) || (*p_max < arr[i])) {
			p_max = &(arr[i]);
		}
		ReleaseMutex(mtx);
		Sleep(7);
	}
	WaitForSingleObject(mtx, INFINITE);
	arr[1] += *p_min;
	ReleaseMutex(mtx);
	WaitForSingleObject(t_handle, INFINITE);
	cout << "Наибольший: " << *p_max << ", наименьший: " << *p_min << endl;
	cout << "Сумма чётных (нулевой, второй и так далее) плюс минимальный: " << arr[1] << endl;
	CloseHandle(mtx);
	CloseHandle(t_handle);
	free(arr);
	system("pause");
	return 0;
}
