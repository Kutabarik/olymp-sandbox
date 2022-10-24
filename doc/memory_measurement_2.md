# Как замерять потребляемую память?
## Windows

Some of the above values are easily available from the appropriate Win32 API, I just list them here for completeness. Others, however, need to be obtained from the Performance Data Helper library (PDH), which is a bit "unintuitive" and takes a lot of painful trial and error to get to work. (At least it took me quite a while, perhaps I've been only a bit stupid...)

Note: for clarity all error checking has been omitted from the following code. Do check the return codes...!

Total Virtual Memory:

```c++
#include "windows.h"

MEMORYSTATUSEX memInfo;
memInfo.dwLength = sizeof(MEMORYSTATUSEX);
GlobalMemoryStatusEx(&memInfo);
DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
```

Note: The name "TotalPageFile" is a bit misleading here. In reality this parameter gives the "Virtual Memory Size", which is size of swap file plus installed RAM.

Virtual Memory currently used:

Same code as in "Total Virtual Memory" and then

 ```c++
 DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
 ```

Virtual Memory currently used by current process:

```c++
#include "windows.h"
#include "psapi.h"

PROCESS_MEMORY_COUNTERS_EX pmc;
GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
```

Total Physical Memory (RAM):

Same code as in "Total Virtual Memory" and then

```c++
DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
```

Physical Memory currently used:

Same code as in "Total Virtual Memory" and then

```c++
DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
```
Physical Memory currently used by current process:

Same code as in "Virtual Memory currently used by current process" and then

```c++
SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
```

CPU currently used:

```c++
#include "TCHAR.h"
#include "pdh.h"

static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;

void init(){
    PdhOpenQuery(NULL, NULL, &cpuQuery);
    // You can also use L"\\Processor(*)\\% Processor Time" and get individual CPU values with PdhGetFormattedCounterArray()
    PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
    PdhCollectQueryData(cpuQuery);
}

double getCurrentValue(){
    PDH_FMT_COUNTERVALUE counterVal;

    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    return counterVal.doubleValue;
}
```

CPU currently used by current process:

```c++
#include "windows.h"

static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

void init(){
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;

    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&lastCPU, &ftime, sizeof(FILETIME));

    self = GetCurrentProcess();
    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

double getCurrentValue(){
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = (sys.QuadPart - lastSysCPU.QuadPart) +
        (user.QuadPart - lastUserCPU.QuadPart);
    percent /= (now.QuadPart - lastCPU.QuadPart);
    percent /= numProcessors;
    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;

    return percent * 100;
}
```
## Linux

On Linux the choice that seemed obvious at first was to use the POSIX APIs like getrusage() etc. I spent some time trying to get this to work, but never got meaningful values. When I finally checked the kernel sources themselves, I found out that apparently these APIs are not yet completely implemented as of Linux kernel 2.6!?

In the end I got all values via a combination of reading the pseudo-filesystem /proc and kernel calls.

Total Virtual Memory:

```c++
#include "sys/types.h"
#include "sys/sysinfo.h"

struct sysinfo memInfo;

sysinfo (&memInfo);
long long totalVirtualMem = memInfo.totalram;
//Add other values in next statement to avoid int overflow on right hand side...
totalVirtualMem += memInfo.totalswap;
totalVirtualMem *= memInfo.mem_unit;
```
Virtual Memory currently used:

Same code as in "Total Virtual Memory" and then

```c++
long long virtualMemUsed = memInfo.totalram - memInfo.freeram;
//Add other values in next statement to avoid int overflow on right hand side...
virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
virtualMemUsed *= memInfo.mem_unit;
```

Virtual Memory currently used by current process:

```c++
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

int getValue(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmSize:", 7) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}
```

Total Physical Memory (RAM):

Same code as in "Total Virtual Memory" and then

```c++
long long totalPhysMem = memInfo.totalram;
//Multiply in next statement to avoid int overflow on right hand side...
totalPhysMem *= memInfo.mem_unit;
```

Physical Memory currently used:

Same code as in "Total Virtual Memory" and then

```c++
long long physMemUsed = memInfo.totalram - memInfo.freeram;
//Multiply in next statement to avoid int overflow on right hand side...
physMemUsed *= memInfo.mem_unit;
```

Physical Memory currently used by current process:

Change getValue() in "Virtual Memory currently used by current process" as follows:

```c++
int getValue(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}
```

CPU currently used:

```c++
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

void init(){
    FILE* file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow,
        &lastTotalSys, &lastTotalIdle);
    fclose(file);
}

double getCurrentValue(){
    double percent;
    FILE* file;
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

    file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow,
        &totalSys, &totalIdle);
    fclose(file);

    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
        totalSys < lastTotalSys || totalIdle < lastTotalIdle){
        //Overflow detection. Just skip this value.
        percent = -1.0;
    }
    else{
        total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
            (totalSys - lastTotalSys);
        percent = total;
        total += (totalIdle - lastTotalIdle);
        percent /= total;
        percent *= 100;
    }

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

    return percent;
}
```

CPU currently used by current process:

```c++
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/times.h"
#include "sys/vtimes.h"

static clock_t lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;

void init(){
    FILE* file;
    struct tms timeSample;
    char line[128];

    lastCPU = times(&timeSample);
    lastSysCPU = timeSample.tms_stime;
    lastUserCPU = timeSample.tms_utime;

    file = fopen("/proc/cpuinfo", "r");
    numProcessors = 0;
    while(fgets(line, 128, file) != NULL){
        if (strncmp(line, "processor", 9) == 0) numProcessors++;
    }
    fclose(file);
}

double getCurrentValue(){
    struct tms timeSample;
    clock_t now;
    double percent;

    now = times(&timeSample);
    if (now <= lastCPU || timeSample.tms_stime < lastSysCPU ||
        timeSample.tms_utime < lastUserCPU){
        //Overflow detection. Just skip this value.
        percent = -1.0;
    }
    else{
        percent = (timeSample.tms_stime - lastSysCPU) +
            (timeSample.tms_utime - lastUserCPU);
        percent /= (now - lastCPU);
        percent /= numProcessors;
        percent *= 100;
    }
    lastCPU = now;
    lastSysCPU = timeSample.tms_stime;
    lastUserCPU = timeSample.tms_utime;

    return percent;
}
```
