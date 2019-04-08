#include <iostream>
#include <locale.h>
#include <Windows.h>

using namespace std;

// У нас потоки используют общую память (пишут в один и тот же массив).
// Необходимо как-то разделить доступ потоков к этой памяти.
// Самый простой способ -- мьютекс.
HANDLE hMutex;

DWORD WINAPI worker(LPVOID params)
{
	int* pdata = (int*)params;
	int size = pdata[0];
	int* min = nullptr;
	for (int i = 2; i < size + 2; i++)
	{
		WaitForSingleObject(hMutex, INFINITE);
		if (i % 2 == 0)
			pdata[1] +=pdata[i];
		ReleaseMutex(hMutex);
		Sleep(12);
	}
	return 0;
}

int main(int argc, char** argv) {
	setlocale(LC_ALL, "RU");
	HANDLE hThread;
	DWORD idTHread;
	hMutex = CreateMutex(NULL, false, NULL); 
	int d_len = 0;
	cout << "Введите размер массива:" << endl;
	cin >> d_len;
	int* datastruct = (int*)malloc(sizeof(int)*(2 + d_len));
	datastruct[0] = d_len;
	datastruct[1] = 0;
	cout << "Введите элементы массива:" << endl;
	for (int i = 2; i < d_len + 2; i++)
		cin >> datastruct[i];
	hThread = CreateThread(NULL, 0, worker, (void*)datastruct, 0, &idTHread);
	int* p_min = nullptr;
	int* p_max = nullptr;
	for (int i = 2; i < d_len + 2; i++)
	{
		WaitForSingleObject(hMutex, INFINITE);
		if ((p_min == nullptr) || (*p_min > datastruct[i])) {
			p_min = &(datastruct[i]);
		}
		ReleaseMutex(hMutex);
		Sleep(7);
		WaitForSingleObject(hMutex, INFINITE);
		if ((p_max == nullptr) || (*p_max < datastruct[i])) {
			p_max = &(datastruct[i]);
		}
		ReleaseMutex(hMutex);
		Sleep(7);
	}
	WaitForSingleObject(hThread, INFINITE);
	cout << "Наибольший элемент массива: " << *p_max << endl;
	cout << "Наименьший элемент массива: " << *p_min << endl;
	cout << "Сумма элементов массива с чётными индексами: " << datastruct[1] << endl;
	CloseHandle(hMutex);
	CloseHandle(hThread);
	free(datastruct);
	system("pause");
	return 0;
}
