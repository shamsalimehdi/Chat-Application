#include <windows.h>
#include <iostream>
#include <thread>

#define BUFFSIZE 100
#define Ready 1
#define NotReady -1

struct Memory {
    char buff[BUFFSIZE];
    int status;
    DWORD pid1;
    DWORD pid2;
};

Memory* shmptr;
HANDLE hMapFile;
HANDLE hEvent1, hEvent2;

void receiveMessages() {
    while (true) {
        WaitForSingleObject(hEvent2, INFINITE);
        std::cout << "Received from User1: " << shmptr->buff << std::endl;
        shmptr->status = NotReady;
        ResetEvent(hEvent2);
    }
}

int main() {
    DWORD pid = GetCurrentProcessId();

    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, TEXT("SharedMemory"));
    if (hMapFile == NULL) {
        std::cerr << "Could not open file mapping object (" << GetLastError() << ")." << std::endl;
        return 1;
    }
    shmptr = (Memory*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(Memory));
    if (shmptr == NULL) {
        std::cerr << "Could not map view of file (" << GetLastError() << ")." << std::endl;
        CloseHandle(hMapFile);
        return 1;
    }

    hEvent1 = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("Event1"));
    if (hEvent1 == NULL) {
        std::cerr << "OpenEvent1 failed (" << GetLastError() << ")." << std::endl;
        return 1;
    }
    hEvent2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("Event2"));
    if (hEvent2 == NULL) {
        std::cerr << "OpenEvent2 failed (" << GetLastError() << ")." << std::endl;
        return 1;
    }

    shmptr->pid2 = pid;
    shmptr->status = NotReady;

    std::thread receiver(receiveMessages);
    receiver.detach();

    while (true) {
        std::cout << "User2: ";
        std::cin.getline(shmptr->buff, BUFFSIZE);
        shmptr->status = Ready;
        SetEvent(hEvent1);

        while (shmptr->status == Ready) {
            Sleep(10); // Prevent busy-waiting
        }
    }

    UnmapViewOfFile(shmptr);
    CloseHandle(hMapFile);
    CloseHandle(hEvent1);
    CloseHandle(hEvent2);

    return 0;
}
