// cs1.6.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>


BOOL FindAdder(HANDLE hProcess, LPVOID lpBaseAdder, LPVOID lpBuffer, SIZE_T nSize)
{
    DWORD oldProtect;
    if (0 == VirtualProtectEx(hProcess, lpBaseAdder, nSize, PAGE_EXECUTE_READWRITE, &oldProtect))
    {
        printf("VirtualProtectEx Error!\n");
        return -1;
    }
    else printf("VirtualProtectEx Suceess\n");

    if (0 == ReadProcessMemory(hProcess, lpBaseAdder, lpBuffer, nSize, NULL))
    {
        printf("ReadProcessMemory Error!\n");
        return -1;
    }
    else printf("ReadProcessMemory Suceess\n");

    if (0 == VirtualProtectEx(hProcess, lpBaseAdder, nSize, oldProtect, &oldProtect))
    {
        printf("VirtualProtectEx Error!\n");
        return -1;
    }
    else printf("VirtualProtectEx Suceess\n");

    return 0;
}


LPVOID FindAdder_Array(HANDLE hProcess, LPVOID lpBaseAdder, DWORD* offest, SIZE_T nSize)
{
    LPVOID lpCurAdder = lpBaseAdder;
    for (int i = 0; i < nSize-1; i++)
    {
        DWORD lpAdder = (DWORD)lpCurAdder + offest[i];
        printf("lpCurAdder = %p\n", lpCurAdder);
        printf("offest = %x\n", offest[i]);
        printf("lpAdder = %x\n", lpAdder);
        if (0 != FindAdder(hProcess, (LPVOID)lpAdder, &lpCurAdder, sizeof(lpCurAdder)))
            return NULL;
        //printf("lpCurAdder = %p\n", lpCurAdder);
    }

    return (LPVOID)((DWORD)lpCurAdder + offest[nSize-1]);
}



LPVOID FindAdder_Array_1(HANDLE hProcess, LPVOID lpBaseAdder, DWORD* offest, SIZE_T nSize)
{
    LPVOID lpAdder = lpBaseAdder;
    for (int i = 0; i < nSize; i++)
    {
        if (0 != FindAdder(hProcess, lpAdder, &lpAdder, sizeof(lpAdder)))
            return NULL;
        lpAdder = (LPVOID)((DWORD)lpAdder + offest[i]);
    }

    return lpAdder;
}


BOOL WirteAdder(HANDLE hProcess, LPVOID lpBaseAdder, LPVOID lpBuffer, SIZE_T nSize)
{
    DWORD oldProtect;
    if (0 == VirtualProtectEx(hProcess, lpBaseAdder, nSize, PAGE_EXECUTE_READWRITE, &oldProtect))
    {
        printf("VirtualProtectEx Error!\n");
        return -1;
    }
    else printf("VirtualProtectEx Suceess\n");

    if (0 == WriteProcessMemory(hProcess, lpBaseAdder, lpBuffer, nSize, NULL))
    {
        printf("WriteProcessMemory Error!\n");
        return -1;
    }
    else printf("WriteProcessMemory Suceess\n");

    if (0 == VirtualProtectEx(hProcess, lpBaseAdder, nSize, oldProtect, &oldProtect))
    {
        printf("VirtualProtectEx Error!\n");
        return -1;
    }
    else printf("VirtualProtectEx Suceess\n");

    return 0;
}


int main()
{
    std::cout << "Hello World!\n";

    char lpBuff[50];
    HWND hWindows = FindWindowA(NULL, "Counter-Strike");
    if (NULL == hWindows)
    {
        printf("FindWindowA Error!\n");
        return -1;
    }
    else
    {
        GetWindowTextA(hWindows, (LPSTR)lpBuff, 50);
        printf("hWindows = %s\n", lpBuff);
    }
        

    DWORD processId;
    DWORD threadId = GetWindowThreadProcessId(hWindows, &processId);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, processId);
    if (NULL == hProcess)
    {
        printf("OpenProcess Error!\n");
        return -1;
    }
    else printf("hProcess = %d\n", processId);

    
    LPVOID baseAdd = (LPVOID)0x25069BC;
    DWORD offest[] = {0x7c, 0x04, 0x160};
    LPVOID endAdd = FindAdder_Array_1(hProcess, baseAdd, offest, sizeof(offest)/sizeof(offest[0]));
    if (NULL == endAdd)
    {
        printf("FindAdder_Array Error!\n");
        return -1;
    }

    printf("%p\n", endAdd);
    FLOAT hp = 200;
    while (TRUE)
    {
        if (-1 == WirteAdder(hProcess, endAdd, &hp, sizeof(hp)))
        {
            printf("WriteAdder Error!\n");
            return -1;
        }
    }
    
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
