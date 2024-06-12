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
        WaitForSingleObject(hEvent1, INFINITE);
        std::cout << "Received from User2: " << shmptr->buff << std::endl;
        shmptr->status = NotReady;
        ResetEvent(hEvent1);
    }
}

int main() {
    DWORD pid = GetCurrentProcessId();

    hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Memory), TEXT("SharedMemory"));
    if (hMapFile == NULL) {
        std::cerr << "Could not create file mapping object (" << GetLastError() << ")." << std::endl;
        return 1;
    }
    shmptr = (Memory*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(Memory));
    if (shmptr == NULL) {
        std::cerr << "Could not map view of file (" << GetLastError() << ")." << std::endl;
        CloseHandle(hMapFile);
        return 1;
    }

    hEvent1 = CreateEvent(NULL, TRUE, FALSE, TEXT("Event1"));
    if (hEvent1 == NULL) {
        std::cerr << "CreateEvent1 failed (" << GetLastError() << ")." << std::endl;
        return 1;
    }
    hEvent2 = CreateEvent(NULL, TRUE, FALSE, TEXT("Event2"));
    if (hEvent2 == NULL) {
        std::cerr << "CreateEvent2 failed (" << GetLastError() << ")." << std::endl;
        return 1;
    }

    shmptr->pid1 = pid;
    shmptr->status = NotReady;

    std::thread receiver(receiveMessages);
    receiver.detach();

    while (true) {
        std::cout << "User1: ";
        std::cin.getline(shmptr->buff, BUFFSIZE);

        shmptr->status = Ready;
        SetEvent(hEvent2);

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
