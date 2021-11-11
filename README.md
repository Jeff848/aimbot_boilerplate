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
DLL Injection is the technique where attackers can run code in the address code of another process by forcing it to load a dynamic-link library. In effect, it allows the attacker to run arbitrary code in arbitrary processes. On Windows, there are multiple ways to do this: changing the DLLs listed in the registry entry `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows\AppInit_DLL`, changing the DLLS listed in the registry key `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\AppCertDLLs`, and through process manipulation functions in the Windows API such as CreateRemoteThread, which we will use with our DLL Injector.

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

Here is an overview of the classes and their respective member functions that are present in this project. Not included in the diagram are Offsets header file which has all of the CSGO offsets from [hazedumper](https://github.com/frk1/hazedumper), and the Source.c and Source.h files which contain our DLL boilerplate code and a Run function for calling while the cheat is running.

### DllMain (Source.c)
Source.c contains our DllMain code, which serves as an entrypoint into the dll code when the dll is loaded into the process. In the case that it is attached to a process, we call a OnDllAttach function, which launches a console for testing purposes, and calls the Run function in an infinite loop which can be stopped when the delete key is entered. Once stopped, the Dll unloads itself and terminates all threads.

### Run (Source.h)
Run gets the target we want to aim at (closest enemy player) and aims at them, using various static and member functions of the LocalPlayer and Player classes. The details for the implementation of the Run function and its helper function, GetClosestEnemy, are detailed in the TODO section below.

### hazedumper (Offsets.h)
The hazedumper namespace contains all of the offsets we need to find the locations of player ViewAngle, player VectorOrigin, etc. in memory. See pointer magic below.

### LocalPlayer
A class that represents you the player. Has some important functions such as GetDistance and AimAt which will be needed for implementing the Run function.

### Player
A class that represents other players. Other than the functions universal to both LocalPlayer and Player, has a function to get the Bone Position of a specific bone in the BoneMatrix. This is needed for aiming at enemy heads.

### Vector3
Simple class for a tuple of floats. Vector3s can represent where things are in 3D space, and are thus needed for our calculations for aiming the bot.

### Pointer magic
The code below shows the general process of getting a pointer to a value in memory.

![image](https://user-images.githubusercontent.com/25162602/141227111-d1cbc290-fe8c-465d-8a98-8b3cf6f96a71.png)

In this code snippet, the this pointer is a pointer to our LocalPlayer object in memory (obtained with the Get method). After casting this to a uintptr_t pointer type (generic 32 bit pointer), we can then dereference it to get to the actual LocalPlayer data structure in memory. Then, by adding the hazedumper offset for the Vector Origin, we can then find the pointer to the LocalPlayer's Vector Origin, which is a Vector3.


## TODOs
Because this boilerplate code is meant to teach the techniques for making an aimbot, several functions are incomplete and are marked TODO. The purpose of these functions is outlined below. Almost all the functions are written out on the slides if you are stuck.

### LocalPlayer::GetDistance
Given the Vector3 indicating something´s position, return the distance between the LocalPlayer and the other Vector3. HINT: how do you find the distance between two points?

### LocalPlayer::AimAt
Given the Vector3 of the target we want to aim at and the viewangles, change the viewangles such that the player now looks toward the target. See slides for details on pitch and yaw.

### Player::GetPlayer
Given the entitylist location and the Player's index in the list, return the Player's pointer.

### Player::GetBonePos
Given the boneMatrix start position and the specific boneId we are looking for, return a Vector3 containing the x, y, and y coordinates of the specific bone. Note that the offset to the x coordinate in the bone matrix is 0xC. HINT: What is the size of a bonematrix? What is the offset to the y and z positions?

### GetClosestEnemy
Loop through all the enemies in the entity list (ie ignoring friendly players, dead players and yourself). Then get the enemy with the least distance between itself and the player and return the enemy.

### Run
Get the closest available enemy. If it exists, then aim at it. This function should be fairly simple if everything else has been propely implemented.

---

## Essential Links
- [Cyber Discord](https://discord.gg/j9dgf2q)
- [Cyber Linktree](https://linktr.ee/uclacyber)
- [Studio Discord](https://discord.com/invite/bBk2Mcw)
- [Studio Linktree](https://linktr.ee/acmstudio)
- [ACM Membership Portal](https://members.uclaacm.com/)
## Additional Resources
- [Source Engine SDK](https://github.com/ValveSoftware/source-sdk-2013)
- [ACM Website](https://www.uclaacm.com/)
- [ACM Discord](https://discord.com/invite/eWmzKsY)
 
 
 
