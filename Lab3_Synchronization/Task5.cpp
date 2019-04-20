#include <iostream>
#include <string>
#include <windows.h>

std::string str; // Global variable, as needed
const int MAX_ITER = 25;
const int MAX_THREADS = 3;
const char* EVENT_NAME[] = {"EV_A", "EV_B", "EV_C", "EV_D", "EV_E", "EV_F", "EV_G", "EV_H", "EV_I", "EV_J"}; // Some names for events
int ints[]  = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }; // There was a problem about passing incremented counter by reference, so that's the way I've made it


DWORD WINAPI thread_writer(LPVOID arg)
{
	int th_num = *static_cast<int*>(arg);

	//Making loop-linked list of events, so threads will iterate one-by-one (after last comes zero again)

	HANDLE dupPrevEvent = OpenEvent(EVENT_MODIFY_STATE | SYNCHRONIZE, false, TEXT(EVENT_NAME[th_num]));
	HANDLE dupNextEvent;
	if (th_num == MAX_THREADS - 1)
		dupNextEvent = OpenEvent(EVENT_MODIFY_STATE | SYNCHRONIZE, false, TEXT(EVENT_NAME[0]));
	else
		dupNextEvent = OpenEvent(EVENT_MODIFY_STATE | SYNCHRONIZE, false, TEXT(EVENT_NAME[th_num + 1]));

	for (int i = 0; i < MAX_ITER; i++)
	{
		WaitForSingleObject(dupPrevEvent, INFINITE);
		ResetEvent(dupPrevEvent);
		str = "This is a thread with number " + std::to_string(th_num) + ", iteartion " + std::to_string(i);
		std::cout << str << std::endl;
		SetEvent(dupNextEvent);
	}
	return 0;
}


int main()
{
	HANDLE threadsHandle[MAX_THREADS];
	DWORD threadIDs[MAX_THREADS];
	HANDLE strEvents[MAX_THREADS];
	for (int i = 0; i < MAX_THREADS; i++)
	{
		strEvents[i] = CreateEvent(NULL, true, false, TEXT(EVENT_NAME[i])); // All events are created non-signaling
		if (strEvents[i] == NULL) {
			std::cout << "Event creation error!" << std::endl;
			std::cout << GetLastError() << std::endl;
			system("pause");
			return -1;
		}
		threadsHandle[i] = CreateThread(NULL, 0, thread_writer, &(ints[i]), 0, &(threadIDs[i]));
		if (threadsHandle[i] == NULL)
		{
			std::cout << "Thread creation error!" << std::endl;
			std::cout << GetLastError() << std::endl;
			for (int i = 0; i < MAX_THREADS; i++)
				CloseHandle(strEvents[i]);
			system("pause");
			return -2;
		}
	}
	SetEvent(strEvents[0]); // To start our event conveyor the very first is turned to signaling state
	DWORD wResult = WaitForMultipleObjects(MAX_THREADS, threadsHandle, true, INFINITE);
	if (wResult == WAIT_OBJECT_0)
	{
		std::cout << "Waiting successful. Program finishes normally." << std::endl;
		for (int i = 0; i < MAX_THREADS; i++)
		{
			CloseHandle(threadsHandle[i]);
			CloseHandle(strEvents[i]);
		}
		system("pause");
		return 0;
	}
	else
	{
		std::cout << "Waiting error!" << std::endl;
		std::cout << GetLastError() << std::endl;
		for (int i = 0; i < MAX_THREADS; i++)
		{
			CloseHandle(threadsHandle[i]);
			CloseHandle(strEvents[i]);
		}
		system("pause");
		return -3;
	}
}
