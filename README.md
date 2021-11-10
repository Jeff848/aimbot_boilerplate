# Cyber x Studio Collab Aimbot Tutorial 
  
**Date**: October 26, 2021, 7:00 pm - 9:00 pm<br>
**Location**: Faraday Room 67-124 (Engineering IV)<br>
 
## Resources
[Slides]()<br>
[Video]()
 
## Topics Covered
* DLL Injection
* Finding Location of Objects in Memory Using HazeDumper Offsets 
* Aimbot Math (pitch and yaw)
 
## What you'll need
* [Git](https://git-scm.com/downloads)
* [Visual Studio](https://visualstudio.microsoft.com/downloads/)
* [CSGO](https://store.steampowered.com/app/730/CounterStrike_Global_Offensive/)

---

## Overview
This project serves as a boilerplate for a basic CSGO aimbot hack. Note that in order to prevent a VAC ban, launching CSGO with the -insecure flag is recommended.
 
## Setting Up the Project
(This part of the tutorial is geared for Visual Studio 2019) First, clone this repository directly into [visual studio](https://docs.microsoft.com/en-us/visualstudio/get-started/tutorial-open-project-from-repo?view=vs-2022). Then, right click on the Solution Explorer to get to the project settings. In the general configuration properties, change the Configuration Type to "Dynamic Library (.dll)". In the advanced configuration properties, change the Target Extension to .dll and change the Character Set to Multi-Byte Character Set. With the project properly set up, you should be able to build the dll by right clicking on the Solution Explorer and selecting Build, and you should be able to see the dll in the Debug/Release folder of your project.

To set up the dll injector, simply clone [this repository](https://github.com/Jeff848/dll_injection) and build the dll injector separately. Replace the dll path in the code and the aimbot dll should be properly injected. 

---

## DLL Injection Explanation
### What is a DLL?
DLL stands for Dynamic Link Library; it is Microsoft's implementation of a shared library (see ldd on linux), where multiple programs can load in the same library and the library is "linked" at runtime. This is in contrast to static linking, which essentially makes a copy of the library part of the executable. This is particularly useful for games, as it prevents bloated disk space and promotes code reuse/modularization by reusing the same code that would otherwise be compiled over and over directly into different binaries. Dynamic Link Libraries also allow developers to update libraries without affecting binaries that depend on such libraries, as applications do not need to be recompiled.

![Picture of Compilation Process](https://i.stack.imgur.com/ZWg39.png)

### What is DLL Injection?
DLL Injection is the technique where attackers can run code in the address code of another process by forcing it to load a dynamic-link library. In effect, it allows the attacker to run arbitrary code in arbitrary processes. On Windows, there are multiple ways to do this: changing the DLLs listed in the registry entry `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows\AppInit_DLL`, changing the DLLS listed in the registry key `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\AppCertDLLs`, and throug process manipulation functions in the Windows API such as CreateRemoteThread, which we will use with our DLL Injector.

### Injection Process
The [CreateRemoteThread](https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createremotethread) function in the Windows API enables us to arbitrarily create a thread in the virtual address space of another process. Thus, we simply have to create a thread in the target process (csgo.exe) that loads our dll into memory. 

![image](https://user-images.githubusercontent.com/25162602/141060468-df968f2c-878e-4ab2-9450-1ac0b0b85169.png)

The image above illustrates the overall injection process. First, after we acquire the procID of csgo.exe, we call the [OpenProcess](https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocess) function. This returns a Handle for csgo.exe process. In short, a [Handle](https://stackoverflow.com/questions/902967/what-is-a-windows-handle) is a Windows API abstraction of a system resource, usually memory. We need it for when we call Windows API functions for setting up csgo.exe for our CreateRemoteThread call.

Then, we use the [VirtualAllocEx](https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualallocex) to allocate enough memory for the dll path in the target process, giving the process Handle as an argument. Note that the function also returns the address of the allocated memory. We then use this memory location to write our dll's path into the virtual memory of the csgo process using the [WriteProcessMemory](https://docs.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-writeprocessmemory) function, giving the process Handle, the address returned by VirtualAllocEx, our dllPath and dllPath string size as arguments. The reason why we want the dll path in memory of the target process is because we need to use the address of the path as an argument for loading our target library.

Finally, we call [CreateRemoteThread](https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createremotethread) on the Handle, giving the process Handle, allocated memory address and the address of the function [LoadLibraryA](https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibrarya) as arguments. For the reason why we have LoadLibraryA as an argument-- when we create a thread, we need to specify a function for it to run. Thus, we run the LoadLibraryA function, which loads a dll into the address space of the calling process, and pass in the dll path as the argument! Everything comes together, and we have successfully loaded our dll into memory.

---
 
## Overview of the Boilerplate Code
### Classes and Functions
![image](https://user-images.githubusercontent.com/25162602/141064941-d2681469-753f-4d7e-9b2d-3482545d5843.png)

Here is an overview of the classes and their respective member functions that are present in this project. Not included here are Offsets header file which has all of the CSGO offsets from [hazedumper](https://github.com/frk1/hazedumper), and the Source.c and Source.h files which contain our DLL boilerplate code and a Run function for calling while the cheat is running.

---

## Essential Links
- [Cyber Discord]()
- [Cyber Linktree]()
- [Studio Discord](https://discord.com/invite/bBk2Mcw)
- [Studio Linktree](https://linktr.ee/acmstudio)
- [ACM Membership Portal](https://members.uclaacm.com/)
## Additional Resources
- [Unity Documentation](https://docs.unity3d.com/Manual/index.html)
- [ACM Website](https://www.uclaacm.com/)
- [ACM Discord](https://discord.com/invite/eWmzKsY)
 
 
 
