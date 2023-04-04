#include <Windows.h>
#include <iostream>
#include <unistd.h>

time_t FileTime_to_POSIX(FILETIME ft)
{
    FILETIME localFileTime;
    FileTimeToLocalFileTime(&ft,&localFileTime);
    SYSTEMTIME sysTime;
    FileTimeToSystemTime(&localFileTime,&sysTime);
    struct tm tmtime = {0};
    tmtime.tm_year = sysTime.wYear - 1900;
    tmtime.tm_mon = sysTime.wMonth - 1;
    tmtime.tm_mday = sysTime.wDay;
    tmtime.tm_hour = sysTime.wHour;
    tmtime.tm_min = sysTime.wMinute;
    tmtime.tm_sec = sysTime.wSecond;
    tmtime.tm_wday = 0;
    tmtime.tm_yday = 0;
    tmtime.tm_isdst = -1;
    time_t ret = mktime(&tmtime);
    return ret;
}


std::int64_t filetimeToUnixTimestamp(const FILETIME& filetime)
{
    std::int64_t filetimeIntervals = static_cast<std::int64_t>(filetime.dwHighDateTime) << 32 | filetime.dwLowDateTime;

    // Количество 100-наносекундных интервалов, прошедших с 1 января 1970 года в UTC.
    std::int64_t epochIntervals = 116444736000000000ll;

    // Вычитание из значения FILETIME количества интервалов, прошедших между 1 января 1601 года и 1 января 1970 года.
    std::int64_t unixIntervals = filetimeIntervals - epochIntervals;

    // Преобразование интервалов в секунды и округление до целого числа.
    std::int64_t unixSeconds = unixIntervals / 10000000ll;

    return unixSeconds;
}

int main()
{
    // Получаем хэндл текущего процесса
    HANDLE processHandle = GetCurrentProcess();

    FILETIME createTime, exitTime, kernelTime, userTime;

    sleep(1);
    // Получаем время создания, завершения, а также время использования процессорного времени в ядре и в пользовательском режиме
    if (GetProcessTimes(processHandle, &createTime, &exitTime, &kernelTime, &userTime) != 0)
    {
        // Выводим полученные значения в консоль
        std::uint64_t timestamp1 = FileTime_to_POSIX(createTime);
        std::uint64_t timestamp2 = FileTime_to_POSIX(exitTime);
        std::uint64_t timestamp3 = FileTime_to_POSIX(kernelTime);
        std::uint64_t timestamp4 = FileTime_to_POSIX(userTime);
        std::cout << "Create time: " << timestamp1 << std::endl;
        std::cout << "Exit time: " << timestamp2 << std::endl;
        std::cout << "Kernel time: " << timestamp3 << std::endl;
        std::cout << "User time: " << timestamp4 << std::endl;
    }
    else
    {
        std::cerr << "Error getting process times" << std::endl;
    }

    // Закрываем хэндл процесса
    CloseHandle(processHandle);

    return 0;
}
