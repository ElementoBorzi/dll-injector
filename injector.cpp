#include <windows.h>
#include <sys/stat.h>
#include <string>

inline auto IsFileExists(char* filename)
{
    struct stat buffer;   
    return (stat (filename, &buffer) == 0);
}

inline auto ArgvToCmdLine(int argc, char* argv[])
{
    std::string buffer;
    for (int i = 0; i < argc; i++)
        (buffer += argv[i]) += ' ';

    return buffer;
}

inline auto PathToName(std::string path)
{
    auto last = path.find_last_of("\\/");
    if (last != std::string::npos)
        path.erase(0, last + 1);

    return path;
}

inline auto error(const char* text)
{
    MessageBoxA(NULL, text, NULL, MB_OK);
    exit(0);
}

inline bool InjectDll(DWORD dwPID, std::string dllname)
{
    auto res = false;
    if (auto hProcess = OpenProcess(PROCESS_VM_OPERATION|PROCESS_VM_WRITE|PROCESS_VM_READ|PROCESS_CREATE_THREAD|PROCESS_QUERY_INFORMATION, FALSE, dwPID)){
        if (auto lpAddr = VirtualAllocEx(hProcess, NULL, dllname.size(), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)){
            if (WriteProcessMemory(hProcess, lpAddr, dllname.c_str(), dllname.size(), NULL)){
                auto lpLoadLibraryA = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32"), "LoadLibraryA");
                if (auto hThread = CreateRemoteThread(hProcess, NULL, 0, lpLoadLibraryA, lpAddr, 0, NULL)){
                    res = WaitForSingleObject(hThread,10 * 1000) != WAIT_TIMEOUT;
                    CloseHandle(hThread);
                }
            }
            VirtualFreeEx(hProcess, lpAddr, dllname.size(), MEM_FREE);
        }
    }

    return res;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::string str;
        ((str = "Usage: ") += PathToName(*argv)) += " program.exe library.dll [args]";
        error(str.c_str());
    }

    for (int i = 1; i < 3; i++)
    {
        if (!IsFileExists(argv[i]))
        {
            std::string str;
            (str = "Unexist file: ") += argv[i];
            error(str.c_str());
        }
    }

    STARTUPINFOA sInfo = { sizeof(STARTUPINFOA) };
    PROCESS_INFORMATION pInfo;
    std::string cmdline;
    ((cmdline = argv[1]) += ' ') += ArgvToCmdLine(argc - 3, &argv[3]);
    if (!CreateProcessA(argv[1], (char*)cmdline.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo))
        error("CreateProcess fail");

    InjectDll(pInfo.hProcess, argv[2]);

    WaitForSingleObject(pInfo.hProcess, INFINITE);
    CloseHandle(pInfo.hProcess);
    CloseHandle(pInfo.hThread);

    return 0;




}