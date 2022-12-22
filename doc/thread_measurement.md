# Измерение времени исполнения алгоритма в потоке

```c++
_int64 FileTimeToQuadWord(PFILETIME pft)
{
    return(Int64ShllMod32(pft->dwHighDateTime, 32) | pft->dwLowDateTime);
}

void PerformLongOperation ()
{
    FILETIME ftKernelTimeStart, ftKernelTimeEnd;
    FILETIME ftUserTimeStart, ftUserTimeEnd;
    FILETIME ftDummy;

    _int64 qwKernelTimeElapsed, qwUserTimeElapsed, qwTotalTimeElapsed;

    // получаем начальные показатели времени
    GetThreadTimes(GetCurrentThread(), &ftDummy, &ftDummy,
                   &ftKernelTimeStart, &ftUserTimeStart);

    // здесь выполняем сложный алгоритм

    // получаем конечные показатели времени
    GetThreadTimes(GetCurrentThread(), &ftDummy, &ftDummy,
                   &ftKernelTimeEnd, &ftUserTimeEnd);

    // получаем значении времени, затраченного на выполнение ядра и User,
    // преобразуя начальные и конечные показатели времени из FILETIME
    // в учетверенные слова, а затем вычитая начальные показатели из конечных
    qwKernelTimeElapsed = FileTimeToQuadWord(&ftKernelTimeEnd) -
                          FileTimeToQuadWord(&ftKernelTimeStart);

    qwUserTimeElapsed = FileTimeToQuadWord(&ftUserTimeEnd) -
                        FileTimeToQuadWord(&ftUserTimeStart);

    // получаем общее время, складывая время выполнения ядра и User
    qwTotalTimeElapsed = qwKernelTimeElapsed + qwUserTimeElapsed;

    // общее время хранится в qwTotalTimeElapsed

}
```

**qwTotalTimeElapsed** - время, выраженное в интервалах по 100 нс.

=================================================================
```c++
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <unistd.h>

int main()
{
    // Start the timer
    auto start = std::chrono::high_resolution_clock::now();

    // Run the "app" program in a stream
    int result = system("app");

    // Check the return value of the "app" program
    if (result != 0)
    {
        std::cerr << "Error running app" << std::endl;
        return 1;
    }

    // Stop the timer
    auto stop = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time in milliseconds
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

    // Print the elapsed time
    std::cout << "Elapsed time: " << elapsed << " milliseconds" << std::endl;

    return 0;
}
```

This code uses the system function to run the "app" program in a stream. It then checks the return value of the system function to make sure the "app" program ran successfully. If there was an error, it prints an error message and returns an error code. If the "app" program ran successfully, it calculates the elapsed time using the chrono library and prints the result.
