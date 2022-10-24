# Как замерять потребляемую память?

Для получения информации о памяти процесса есть несколько API, наиболее интересны из них:

1. Функция [GetProcessMemoryInfo](https://docs.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-getprocessmemoryinfo)
2. [Счетчики производительности](https://docs.microsoft.com/en-us/windows/win32/perfctrs/performance-counters-portal)
3. Функция [HeapSummary](https://docs.microsoft.com/en-us/windows/win32/api/heapapi/nf-heapapi-heapsummary)

Пример кода:

```c++
#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include "pdh.h"
#pragma comment( lib, "Pdh.lib" )

long GetPrivateWorkingSet(const wchar_t* process) {

    PDH_HQUERY query;
    PDH_HCOUNTER counter;
    PDH_FMT_COUNTERVALUE counterVal;
    const wchar_t* name = L"\\Process(%s)\\Working Set - Private";
    wchar_t buf[1024]=L"";
    swprintf_s(buf, 1024, name, process);

    PdhOpenQuery(NULL, NULL, &query);
    PDH_STATUS res = PdhAddEnglishCounter(query, buf, NULL, &counter);
    res = PdhCollectQueryData(query);       
    PdhGetFormattedCounterValue(counter, PDH_FMT_LONG, NULL, &counterVal);
    long ret = counterVal.longValue;
    PdhCloseQuery(query);

    return ret;
}

void PrintMemory() {
    
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    printf("Private committed memory: %u B\nWorking set: %u B\n", (UINT)pmc.PrivateUsage, (UINT)pmc.WorkingSetSize);

    printf("Private working set: %u B\n", (UINT)GetPrivateWorkingSet(L"Program"));

    HANDLE heaps[20];
    HEAP_SUMMARY hs;

    DWORD cHeaps = GetProcessHeaps(20, heaps);

    for (DWORD i = 0; i < cHeaps; i++) {
        memset(&hs, 0, sizeof(hs));
        hs.cb = sizeof(hs);
        if (HeapSummary(heaps[i], 0, &hs) == FALSE) printf("HeapSummary failed");
        printf("Heap #%u size: %u B\n", (UINT)i, (UINT)hs.cbAllocated);
    }
    printf("\n");
}

int main()
{
    //первый запуск нужен для прогрева, т.е чтобы инициализировать внутренние структуры API
    //(так как API счетчиков производительности сам выделяет память при первом запуске)
    PrintMemory();

    //второй запуск, соответственно, дает достоверные значения
    PrintMemory();

    getchar();
}
```

__Private committed memory__ (Частная выделенная память) - объем виртуальной памяти процесса в состоянии MEM_COMMIT, принадлежащей исключительно этому процессу. Состояние MEM_COMMIT значит, что под данную память выделено место в файле подкачки, а в физической памяти оно может быть либо выделено, либо нет. Из данного значения исключена память, которая является разделяемой между разными процессами - отображенные на память файлы и загруженные DLL, отсюда "private". Звучит сложно, но по сути это и есть главный показатель, сколько процесс "жрет" памяти.

__Working set__ (Рабочий набор) - общий объем физической памяти, выделенной процессу, в том числе разделяемой между несколькими процессами.

__Private working set__ (Частный рабочий набор) - объем физической памяти, принадлежащей исключительно данному процессу. Этот показатель более полезен, чем предыдущий, но получить его сложнее.

__Heap size__ - объем выделенной динамической памяти (помимо непосредственно выделенного нами через new, включает также то, что выделено внутренне библиотеками).