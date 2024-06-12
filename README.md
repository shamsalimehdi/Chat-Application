# Chat-Application
# Chat Application on console in c++ using shared memory having four members BSIT(SS1)
# SHAMS BSIT51F22S016
# ABDUL HADI BSIT51F22S017
# ABDUL REHMAN BSIT51F22S002
# NABEEL PERVAIZ BSIT51F22S031
**Overview**

This repository contains a simple message-passing application using shared memory and event synchronization on Windows. The application consists of two separate programs, User1 and User2, which communicate with each other by writing to and reading from a shared memory segment.

**Structure The project includes the following files:**

**User1.cpp:** The source code for the first user application.

**User2.cpp:** The source code for the second user application.

**README.md:** This README file.

**Requirements**

  Windows operating system
  
  C++ compiler (such as MSVC)
  
**Building the Applications Open a terminal or command prompt.**

  Compile User1.cpp.
  
  Compile User2.cpp.
  
**Running the Applications**

Start User1.exe in one terminal:User1.exe

Start User2.exe in another terminal:User2.exe

**How It Works**

User1.cpp Creates a shared memory segment using CreateFileMapping. Maps the shared memory into the process's address space using MapViewOfFile. Creates two events (Event1 and Event2) for synchronization using CreateEvent. Starts a thread to listen for messages from User2. In the main loop, reads input from the console, writes it to the shared memory, and signals User2 via Event2.

User2.cpp Opens the existing shared memory segment using OpenFileMapping. Maps the shared memory into the process's address space using MapViewOfFile. Opens the existing events (Event1 and Event2) for synchronization using OpenEvent. Starts a thread to listen for messages from User1. In the main loop, reads input from the console, writes it to the shared memory, and signals User1 via Event1. Shared Memory and Synchronization

The shared memory segment is a Memory structure containing: char buff[BUFFSIZE]: A buffer for the message. int status: A status flag to indicate whether the buffer is ready to be read. DWORD pid1 and DWORD pid2: Process IDs of User1 and User2. Events (Event1 and Event2) are used to signal when a new message is available.

**Example **

User1 enters a message and presses Enter. User1 writes the message to the shared memory, sets the status to Ready, and signals User2 using Event2. User2 receives the signal, reads the message from the shared memory, prints it to the console, and resets the status to NotReady. Steps 1-3 are repeated for User2 to send a message back to User1 using Event1.

**Note **

Both applications should be started in separate terminals for proper communication. Make sure the events and shared memory are correctly created and opened by checking the error messages.
